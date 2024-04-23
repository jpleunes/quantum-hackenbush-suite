#ifndef QUANTUM_HACKENBUSH_H
#define QUANTUM_HACKENBUSH_H

#include <limits>
#include <algorithm>

#include "Superposition/Superposition.h"
#include "../Util/Generator.h"
#include "DyadicRational.h"

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
    std::optional<bool> isNumber;
    std::optional<DyadicRational> value;
    std::optional<size_t> birthday;
};

/**
 * An abstract class representing a flavour of Quantum Hackenbush.
 */
template<typename Realisation>
class QuantumHackenbush {
public:
    QuantumHackenbush(const Superposition<Realisation> superposition);
    const Superposition<Realisation>& getSuperposition() const { return superposition; }

    virtual std::vector<GameInstanceId> getMoveOptions(Player player) const = 0;

    template<typename Flavour>
    OutcomeClass determineOutcomeClass() const {
        if (cache.outcome.has_value()) return cache.outcome.value();

        auto leftStartOutcome = determineOutcomeClass<Flavour>(Player::LEFT);
        auto rightStartOutcome = determineOutcomeClass<Flavour>(Player::RIGHT);

        bool leftHasWinningMove = leftStartOutcome == OutcomeClass::L || leftStartOutcome == OutcomeClass::P;
        bool rightHasWinningMove = rightStartOutcome == OutcomeClass::R || rightStartOutcome == OutcomeClass::P;
        if (leftHasWinningMove && rightHasWinningMove) {
            cache.outcome = OutcomeClass::N;
            return OutcomeClass::N;
        }
        if (leftHasWinningMove) {
            cache.outcome = OutcomeClass::L;
            return OutcomeClass::L;
        }
        else if (rightHasWinningMove) {
            cache.outcome = OutcomeClass::R;
            return OutcomeClass::R;
        }
        else {
            cache.outcome = OutcomeClass::P;
            return OutcomeClass::P;
        }
    }

    template<typename Flavour>
    size_t determineBirthday(const std::vector<GameInstanceId>& moveOptionIds) const {
        if (cache.birthday.has_value()) return cache.birthday.value();
        size_t birthday = 0;
        if (!moveOptionIds.empty()) {
            for (GameInstanceId moveOptionId : moveOptionIds)
                birthday = std::max(birthday, GameInstanceDatabase<Flavour>::getInstance().getGameInstance(moveOptionId).template determineBirthday<Flavour>());
        }
        else {
            std::vector<GameInstanceId> leftMoveOptionIds = getMoveOptions(Player::LEFT);
            std::vector<GameInstanceId> rightMoveOptionIds = getMoveOptions(Player::RIGHT);
            if (leftMoveOptionIds.empty() && rightMoveOptionIds.empty()) {
                cache.birthday = 0;
                return 0;
            }
            for (GameInstanceId moveOptionId : leftMoveOptionIds)
                birthday = std::max(birthday, GameInstanceDatabase<Flavour>::getInstance().getGameInstance(moveOptionId).template determineBirthday<Flavour>());
            for (GameInstanceId moveOptionId : rightMoveOptionIds)
                birthday = std::max(birthday, GameInstanceDatabase<Flavour>::getInstance().getGameInstance(moveOptionId).template determineBirthday<Flavour>());
        }
        birthday += 1;
        cache.birthday = birthday;
        return birthday;
    }

    template<typename Flavour>
    size_t determineBirthday() const {        
        return determineBirthday<Flavour>(std::vector<GameInstanceId>());
    }

    template<typename Flavour>
    std::optional<DyadicRational> determineValue() const {
        if (cache.isNumber.has_value() && !cache.isNumber.value()) return {};
        if (cache.value.has_value()) return cache.value.value();

        std::vector<GameInstanceId> leftMoveOptionIds = getMoveOptions(Player::LEFT);
        std::vector<GameInstanceId> rightMoveOptionIds = getMoveOptions(Player::RIGHT);
        if (leftMoveOptionIds.empty() && rightMoveOptionIds.empty()) {
            DyadicRational value(0);
            cache.value = value;
            return value;
        }
        else if (leftMoveOptionIds.empty()) {
            // At this point we have already calculated all move options.
            // We will need them again in determineBirthday, so we might as well pass them along.
            DyadicRational value((long long) -determineBirthday<Flavour>(rightMoveOptionIds));
            cache.value = value;
            return value;
        }
        else if (rightMoveOptionIds.empty()) {
            // At this point we have already calculated all move options.
            // We will need them again in determineBirthday, so we might as well pass them along.
            DyadicRational value((long long) determineBirthday<Flavour>(leftMoveOptionIds));
            cache.value = value;
            return value;
        }

        std::vector<DyadicRational> leftValues;
        for (GameInstanceId moveOptionId : leftMoveOptionIds) {
            std::optional<DyadicRational> leftValue = GameInstanceDatabase<Flavour>::getInstance().getGameInstance(moveOptionId).template determineValue<Flavour>();
            if (!leftValue.has_value()) {
                cache.isNumber = false;
                return {};
            }
            leftValues.emplace_back(leftValue.value());
        }
        DyadicRational bestLeftValue = *std::max_element(leftValues.begin(), leftValues.end());

        std::vector<DyadicRational> rightValues;
        for (GameInstanceId moveOptionId : rightMoveOptionIds) {
            std::optional<DyadicRational> rightValue = GameInstanceDatabase<Flavour>::getInstance().getGameInstance(moveOptionId).template determineValue<Flavour>();
            if (!rightValue.has_value()) {
                cache.isNumber = false;
                return {};
            }
            rightValues.emplace_back(rightValue.value());
        }
        DyadicRational bestRightValue = *std::min_element(rightValues.begin(), rightValues.end());

        std::optional<DyadicRational> value = getSimplestNumber(bestLeftValue, bestRightValue);
        cache.isNumber = value.has_value();
        cache.value = value;
        return value;
    }

    virtual ~QuantumHackenbush() = default;

protected:
    template<typename Flavour>
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
                GameInstanceId gameInstanceId = GameInstanceDatabase<Flavour>::getInstance().getGameInstanceId(Flavour(option));
                result.emplace_back(gameInstanceId);
            }
        }
        return result;
    }

    const Superposition<Realisation> superposition;

    mutable GameInstanceCacheBlock cache;

private:
    template<typename Flavour>
    OutcomeClass determineOutcomeClass(Player turn) const {
        if (turn == Player::LEFT && cache.leftStartsOutcome.has_value()) return cache.leftStartsOutcome.value();
        else if (turn == Player::RIGHT && cache.rightStartsOutcome.has_value()) return cache.rightStartsOutcome.value();

        std::vector<GameInstanceId> moveOptionIds = getMoveOptions(turn);
        switch (turn) {
            case Player::LEFT:
                for (GameInstanceId moveOptionId : moveOptionIds) {
                    OutcomeClass outcome = GameInstanceDatabase<Flavour>::getInstance().getGameInstance(moveOptionId).template determineOutcomeClass<Flavour>(Player::RIGHT);
                    if (outcome == OutcomeClass::L || outcome == OutcomeClass::P) {
                        cache.leftStartsOutcome = OutcomeClass::L;
                        return OutcomeClass::L;
                    }
                }
                cache.leftStartsOutcome = OutcomeClass::R;
                return OutcomeClass::R;
            case Player::RIGHT:
                for (GameInstanceId moveOptionId : moveOptionIds) {
                    OutcomeClass outcome = GameInstanceDatabase<Flavour>::getInstance().getGameInstance(moveOptionId).template determineOutcomeClass<Flavour>(Player::LEFT);
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
