#ifndef QUANTUM_HACKENBUSH_H
#define QUANTUM_HACKENBUSH_H

#include <limits>

#include "Superposition/Superposition.h"
#include "../Util/Generator.h"

const int width = 2;

enum class OutcomeClass {
    L,
    R,
    N,
    P,
};

typedef size_t GameInstanceId;
#define ILLEGAL_GAME_INSTANCE_ID std::numeric_limits<GameInstanceId>::max()

#include "GameInstanceDatabase.h" // This include needs to be here, because it depends on GameInstanceId

struct GameInstanceCacheBlock {
    std::optional<OutcomeClass> outcome;
    std::optional<OutcomeClass> leftStartsOutcome;
    std::optional<OutcomeClass> rightStartsOutcome;
};

/**
 * An abstract class representing a ruleset of Quantum Hackenbush.
 */
template<typename Realisation>
class QuantumHackenbush {
public:
    typedef std::vector<typename Realisation::Piece> Move;

    QuantumHackenbush(const Superposition<Realisation> superposition);
    const Superposition<Realisation>& getSuperposition() const { return superposition; }

    virtual std::vector<GameInstanceId> getMoveOptions(Player player) const = 0;

    template<typename Ruleset>
    OutcomeClass determineOutcomeClass() const {
        if (cache.outcome.has_value()) return cache.outcome.value();

        auto leftStartOutcome = determineOutcomeClass<Ruleset>(Player::LEFT);
        auto rightStartOutcome = determineOutcomeClass<Ruleset>(Player::RIGHT);

        // We do not need to check whether both left and right can win when they start, because such 
        // situations are impossible (Hackenbush does not have N-positions).
        if (leftStartOutcome == OutcomeClass::L || leftStartOutcome == OutcomeClass::P) {
            cache.outcome = OutcomeClass::L;
            return OutcomeClass::L;
        }
        else if (rightStartOutcome == OutcomeClass::R || rightStartOutcome == OutcomeClass::P) {
            cache.outcome = OutcomeClass::R;
            return OutcomeClass::R;
        }
        else {
            cache.outcome = OutcomeClass::P;
            return OutcomeClass::P;
        }
    }

    virtual ~QuantumHackenbush() = default;

protected:
    template<typename Ruleset>
    std::vector<GameInstanceId> getSuperposedMoveOptions(Player player) const { // TODO: also allow moves with width >2 (use width as the maximum width?)
        std::vector<typename Realisation::Piece> pieces = superposition.getPieces(player);

        std::vector<GameInstanceId> result;
        if (pieces.size() < width) return result;
        auto indexCombinationsGen = indexCombinations(pieces.size());
        while (indexCombinationsGen) {
            std::vector<size_t> move = indexCombinationsGen();
            Superposition<Realisation> option = Superposition<Realisation>();
            for (PositionId realisationId : superposition.getRealisationIds()) {
                for (size_t pieceIndex : move) {
                    Realisation& realisation = PositionDatabase<Realisation>::getInstance().getPosition(realisationId);
                    PositionId newRealisationId = realisation.applyMove(pieces[pieceIndex]);
                    if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
                }
            }
            if (!option.empty()) {
                GameInstanceId gameInstanceId = GameInstanceDatabase<Ruleset>::getInstance().getGameInstanceId(Ruleset(option));
                result.emplace_back(gameInstanceId);
            }
        }
        return result;
    }

    const Superposition<Realisation> superposition;

    mutable GameInstanceCacheBlock cache;

private:
    template<typename Ruleset>
    OutcomeClass determineOutcomeClass(Player turn) const {
        if (turn == Player::LEFT && cache.leftStartsOutcome.has_value()) return cache.leftStartsOutcome.value();
        if (turn == Player::RIGHT && cache.rightStartsOutcome.has_value()) return cache.rightStartsOutcome.value();

        std::vector<GameInstanceId> moveOptionIds = getMoveOptions(turn);
        switch (turn) {
            case Player::LEFT:
                for (GameInstanceId moveOptionId : moveOptionIds) {
                    OutcomeClass outcome = GameInstanceDatabase<Ruleset>::getInstance().getGameInstance(moveOptionId).template determineOutcomeClass<Ruleset>(Player::RIGHT);
                    if (outcome == OutcomeClass::L || outcome == OutcomeClass::P) {
                        cache.leftStartsOutcome = OutcomeClass::L;
                        return OutcomeClass::L;
                    }
                }
                cache.leftStartsOutcome = OutcomeClass::R;
                return OutcomeClass::R;
            case Player::RIGHT:
                for (GameInstanceId moveOptionId : moveOptionIds) {
                    OutcomeClass outcome = GameInstanceDatabase<Ruleset>::getInstance().getGameInstance(moveOptionId).template determineOutcomeClass<Ruleset>(Player::LEFT);
                    if (outcome == OutcomeClass::R || outcome == OutcomeClass::P) {
                        cache.rightStartsOutcome = OutcomeClass::R;
                        return OutcomeClass::R;
                    }
                }
                cache.rightStartsOutcome = OutcomeClass::L;
                return OutcomeClass::L;
            default:
                throw(std::domain_error("Unknown player case."));
        }
    }

    /// @brief TODO
    /// @param n 
    /// @return 
    Generator<std::vector<size_t>> indexCombinations(size_t n) const {
        // This algorithm is a C++ adaptation of https://github.com/blazs/subsets
        std::vector<size_t> combination(width);
        int i, j, r;

        for (i = 0; i < width; ++i) combination[i] = i; // Initial combination
        while (true) {
            std::vector<size_t> combinationCopy = combination;
            co_yield combinationCopy;

            if (combination[0] == n - width) break;

            for (i = width - 1; i >= 0 && combination[i] + width - i == n; --i);
            r = combination[i];
            ++combination[i];
            j = 2;
            for (++i; i < width; ++i, ++j) combination[i] = r + j;
        }
    }
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbush<Realisation>::QuantumHackenbush(const Superposition<Realisation> superposition) : superposition(superposition) {
}

#endif // QUANTUM_HACKENBUSH_H
