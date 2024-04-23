#ifndef SUPERPOSITION_H
#define SUPERPOSITION_H

#include <vector>
#include <set>
#include <limits>
#include <algorithm>

#include "Position/PositionDatabase.h"
#include "../../Util/HashUtil.h"
#include "Generator.h"
#include "DyadicRational.h"

const int width = 2;

enum class OutcomeClass {
    L,
    R,
    N,
    P,
};

typedef size_t SuperpositionId;
#define ILLEGAL_SUPERPOSITION_ID std::numeric_limits<SuperpositionId>::max()

#include "SuperpositionDatabase.h" // This include needs to be here, because it depends on SuperpositionId

struct SuperpositionCacheBlock {
    std::optional<OutcomeClass> outcome; // Should be moved to CombinatorialGame
    std::optional<OutcomeClass> leftStartsOutcome; // Should be moved to CombinatorialGame
    std::optional<OutcomeClass> rightStartsOutcome; // Should be moved to CombinatorialGame
    std::optional<bool> isNumber; // Should be moved to CombinatorialGame
    std::optional<DyadicRational> value; // Should be moved to CombinatorialGame
    std::optional<size_t> birthday; // Should be moved to CombinatorialGame
};

/**
 * An abstract class representing a Quantum Hackenbush superposition under some flavour.
 */
template<typename Realisation>
class Superposition {
public:
    std::set<PositionId> realisationIds;

    Superposition() = default;
    Superposition(PositionId classicalPositionId);
    bool operator==(const Superposition<Realisation>& other) const { return realisationIds == other.realisationIds; }
    bool empty() const;
    void addRealisationId(PositionId realisationId);
    // Gets the pieces which exist in at least one realisation (and can thus be used in a move) 
    // for a given player.
    std::vector<typename Realisation::Piece> getPieces(Player player) const;

    virtual std::vector<SuperpositionId> getMoveOptions(Player player) const = 0;

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
    size_t determineBirthday(const std::vector<SuperpositionId>& moveOptionIds) const {
        if (cache.birthday.has_value()) return cache.birthday.value();
        size_t birthday = 0;
        if (!moveOptionIds.empty()) {
            for (SuperpositionId moveOptionId : moveOptionIds)
                birthday = std::max(birthday, SuperpositionDatabase<Flavour>::getInstance().getSuperposition(moveOptionId).template determineBirthday<Flavour>());
        }
        else {
            std::vector<SuperpositionId> leftMoveOptionIds = getMoveOptions(Player::LEFT);
            std::vector<SuperpositionId> rightMoveOptionIds = getMoveOptions(Player::RIGHT);
            if (leftMoveOptionIds.empty() && rightMoveOptionIds.empty()) {
                cache.birthday = 0;
                return 0;
            }
            for (SuperpositionId moveOptionId : leftMoveOptionIds)
                birthday = std::max(birthday, SuperpositionDatabase<Flavour>::getInstance().getSuperposition(moveOptionId).template determineBirthday<Flavour>());
            for (SuperpositionId moveOptionId : rightMoveOptionIds)
                birthday = std::max(birthday, SuperpositionDatabase<Flavour>::getInstance().getSuperposition(moveOptionId).template determineBirthday<Flavour>());
        }
        birthday += 1;
        cache.birthday = birthday;
        return birthday;
    }

    template<typename Flavour>
    size_t determineBirthday() const {        
        return determineBirthday<Flavour>(std::vector<SuperpositionId>());
    }

    template<typename Flavour>
    std::optional<DyadicRational> determineValue() const {
        if (cache.isNumber.has_value() && !cache.isNumber.value()) return {};
        if (cache.value.has_value()) return cache.value.value();

        std::vector<SuperpositionId> leftMoveOptionIds = getMoveOptions(Player::LEFT);
        std::vector<SuperpositionId> rightMoveOptionIds = getMoveOptions(Player::RIGHT);
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
        for (SuperpositionId moveOptionId : leftMoveOptionIds) {
            std::optional<DyadicRational> leftValue = SuperpositionDatabase<Flavour>::getInstance().getSuperposition(moveOptionId).template determineValue<Flavour>();
            if (!leftValue.has_value()) {
                cache.isNumber = false;
                return {};
            }
            leftValues.emplace_back(leftValue.value());
        }
        DyadicRational bestLeftValue = *std::max_element(leftValues.begin(), leftValues.end());

        std::vector<DyadicRational> rightValues;
        for (SuperpositionId moveOptionId : rightMoveOptionIds) {
            std::optional<DyadicRational> rightValue = SuperpositionDatabase<Flavour>::getInstance().getSuperposition(moveOptionId).template determineValue<Flavour>();
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

    virtual ~Superposition() = default;

protected:
    template<typename Flavour>
    std::vector<SuperpositionId> getSuperposedMoveOptions(Player player) const { // TODO: also allow moves with width >2 (use width as the maximum width?)
        std::vector<typename Realisation::Piece> pieces = getPieces(player);

        std::vector<SuperpositionId> result;
        if (pieces.size() < width) return result;
        auto indexCombinationsGen = indexCombinations(pieces.size());
        while (indexCombinationsGen) {
            std::vector<size_t> move = indexCombinationsGen();
            Flavour option;
            for (PositionId realisationId : realisationIds) {
                for (size_t pieceIndex : move) {
                    Realisation& realisation = PositionDatabase<Realisation>::getInstance().getPosition(realisationId);
                    PositionId newRealisationId = realisation.applyMove(pieces[pieceIndex]);
                    if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
                }
            }
            if (!option.empty()) {
                SuperpositionId superpositionId = SuperpositionDatabase<Flavour>::getInstance().getSuperpositionId(option);
                result.emplace_back(superpositionId);
            }
        }
        return result;
    }

    mutable SuperpositionCacheBlock cache;
    
private:
    template<typename Flavour>
    OutcomeClass determineOutcomeClass(Player turn) const {
        if (turn == Player::LEFT && cache.leftStartsOutcome.has_value()) return cache.leftStartsOutcome.value();
        else if (turn == Player::RIGHT && cache.rightStartsOutcome.has_value()) return cache.rightStartsOutcome.value();

        std::vector<SuperpositionId> moveOptionIds = getMoveOptions(turn);
        switch (turn) {
            case Player::LEFT:
                for (SuperpositionId moveOptionId : moveOptionIds) {
                    OutcomeClass outcome = SuperpositionDatabase<Flavour>::getInstance().getSuperposition(moveOptionId).template determineOutcomeClass<Flavour>(Player::RIGHT);
                    if (outcome == OutcomeClass::L || outcome == OutcomeClass::P) {
                        cache.leftStartsOutcome = OutcomeClass::L;
                        return OutcomeClass::L;
                    }
                }
                cache.leftStartsOutcome = OutcomeClass::R;
                return OutcomeClass::R;
            case Player::RIGHT:
                for (SuperpositionId moveOptionId : moveOptionIds) {
                    OutcomeClass outcome = SuperpositionDatabase<Flavour>::getInstance().getSuperposition(moveOptionId).template determineOutcomeClass<Flavour>(Player::LEFT);
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
Superposition<Realisation>::Superposition(PositionId classicalPositionId) : realisationIds({classicalPositionId}) {
}

template<typename Realisation>
bool Superposition<Realisation>::empty() const {
    return realisationIds.empty();
}

template<typename Realisation>
void Superposition<Realisation>::addRealisationId(PositionId realisationId) {
    realisationIds.emplace(realisationId);
}

template<typename Realisation>
std::vector<typename Realisation::Piece> Superposition<Realisation>::getPieces(Player player) const {
    std::set<typename Realisation::Piece> pieces;
    for (PositionId realisationId : realisationIds) {
        for (typename Realisation::Piece piece : PositionDatabase<Realisation>::getInstance().getPosition(realisationId).getPieces(player)) {
            pieces.insert(piece);
        }
    }
    return std::vector<typename Realisation::Piece>(pieces.begin(), pieces.end());
}

namespace std {
    template<typename Realisation>
    struct hash<Superposition<Realisation>> {
        size_t operator()(const Superposition<Realisation>& superposition) const {
            std::vector<uint32_t> intVec;
            for (PositionId realisationId : superposition.realisationIds) intVec.push_back((uint32_t) realisationId);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }
    };
}

#endif // SUPERPOSITION_H
