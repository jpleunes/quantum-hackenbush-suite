#ifndef SUPERPOSED_GAME_STATE_H
#define SUPERPOSED_GAME_STATE_H

#include <vector>
#include <set>
#include <limits>
#include <algorithm>

#include "../../ShortGame/ShortGame.h"
#include "GameState/GameStateDatabase.h"
#include "../../Util/HashUtil.h"
#include "Generator.h"
#include "GameState/Position/ShortHollyhocksPosition.h"

typedef size_t SuperposedGameStateId;

#include "SuperposedGameStateDatabase.h" // This include needs to be here, because it depends on SuperposedGameStateId

struct SuperposedGameStateCache {
    std::optional<std::vector<SuperposedGameStateId>> leftOptions, rightOptions;
    std::optional<ShortGameId> shortGameId;
    // TODO: displayString
};

/**
 * An abstract class representing a Hackenbush superposed game state under some flavour.
 */
template<typename Realisation>
class SuperposedGameState {
public:
    // This constructor should only be used from within SuperposedGameStateDatabase.
    // Otherwise, SuperposedGameStateDatabase.getOrInsert should be used.
    SuperposedGameState(GameStateId classicalGameState, SuperposedGameStateId id);
    // This constructor should only be used from within SuperposedGameStateDatabase.
    // Otherwise, SuperposedGameStateDatabase.getOrInsert should be used.
    SuperposedGameState(std::set<GameStateId> realisations, SuperposedGameStateId id);

    const std::set<GameStateId>& getRealisations() const { return realisations; }
    SuperposedGameStateId getId() const { return id; }
    // Checks whether two superposed game states have equal realisations sets.
    bool operator==(const SuperposedGameState<Realisation>& other) const {
        // Superposed game states with equal realisations sets are assigned the same id.
        // Two realisations are considered equal if the repersentations of the positions are equal.
        // Note that equality of position representanions implies that they have equal labelling.
        return id == other.id;
    }
    // Gets the pieces which exist in at least one realisation (and can thus be used in a move) 
    // for a given player.
    std::vector<typename Realisation::Piece> getPieces(Player player) const;

    virtual std::vector<SuperposedGameStateId> getOptions(Player player, size_t width) const = 0;

    // Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
    template<typename Flavour>
    ShortGameId determineShortGameId(size_t width) const {
        if (cache.shortGameId.has_value()) return cache.shortGameId.value();

        std::set<ShortGameId> leftOptions, rightOptions;
        for (SuperposedGameStateId leftOption : getOptions(Player::LEFT, width)) {
            leftOptions.insert(SuperposedGameStateDatabase<Flavour>::getInstance().getSuperposedGameState(leftOption).template determineShortGameId<Flavour>(width));
        }
        for (SuperposedGameStateId rightOption : getOptions(Player::RIGHT, width)) {
            rightOptions.insert(SuperposedGameStateDatabase<Flavour>::getInstance().getSuperposedGameState(rightOption).template determineShortGameId<Flavour>(width));
        }
        ShortGameId result = ShortGameDatabase::getInstance().getOrInsert(leftOptions, rightOptions).getId();

        cache.shortGameId = result;

        return result;
    }

    virtual ~SuperposedGameState() = default;

protected:
    template<typename Flavour>
    std::vector<SuperposedGameStateId> getSuperposedOptions(Player player, size_t width) const {
        std::vector<typename Realisation::Piece> pieces = getPieces(player);

        std::vector<SuperposedGameStateId> result;
        // We cannot make moves that are wider than the number of pieces
        width = std::min(width, pieces.size());
        // Apply all superposed moves of width 2<=w<=width
        for (size_t w = 2; w <= width; w++) {
        auto indexCombinationsGen = indexCombinations(pieces.size(), w);
            while (indexCombinationsGen) {
                std::vector<size_t> move = indexCombinationsGen();
                std::set<GameStateId> option;
                for (GameStateId realisationId : realisations) {
                    for (size_t pieceIndex : move) {
                        GameState<Realisation> realisation = GameStateDatabase<Realisation>::getInstance().getGameState(realisationId);
                        std::optional<GameStateId> newRealisationId = realisation.applyMove(pieces[pieceIndex]);
                        if (newRealisationId.has_value()) option.insert(newRealisationId.value());
                    }
                }
                if (!option.empty()) {
                    SuperposedGameStateId superposedGameStateId = SuperposedGameStateDatabase<Flavour>::getInstance().getOrInsert(option).getId();
                    result.emplace_back(superposedGameStateId);
                }
            }
        }
        return result;
    }

    std::set<GameStateId> determineNonCoveredRealisations(std::set<GameStateId> realisations) {
    std::set<GameStateId> nonCoveredRealisations;
    for (GameStateId realisationId : realisations) {
        GameState<ShortHollyhocksPosition> realisation = GameStateDatabase<ShortHollyhocksPosition>::getInstance().getGameState(realisationId);
        std::set<ShortHollyhocksPiece> pieces = realisation.getPieces();

        // Check if there exists another realisation that we want to keep containing all of this realisation's pieces
        bool covered = false;
        for (GameStateId otherRealisationId : nonCoveredRealisations) {
            GameState<ShortHollyhocksPosition> otherRealisation = GameStateDatabase<ShortHollyhocksPosition>::getInstance().getGameState(otherRealisationId);
            std::set<ShortHollyhocksPiece> otherPieces = otherRealisation.getPieces();
            bool hasNewPiece = false;
            for (ShortHollyhocksPiece piece : pieces) {
                if (!otherPieces.contains(piece)) {
                    hasNewPiece = true;
                    break;
                }
            }
            if (!hasNewPiece) {
                covered = true;
                break;
            }
        }
        if (!covered) nonCoveredRealisations.insert(realisationId);
    }

    return nonCoveredRealisations;
}

    mutable SuperposedGameStateCache cache;
    
private:
    const std::set<GameStateId> realisations;
    const SuperposedGameStateId id;

    /// @brief Gets all lexicographically strictly increasing vectors of indices ranging from 0 to n-1, of size w.
    /// @param n up to which value (exclusive) the indices should range
    /// @param w the size of the combination vectors that should be returned
    /// @return generator yielding vectors representing combinations of indices
    Generator<std::vector<size_t>> indexCombinations(size_t n, size_t w) const {
        // This algorithm is a C++ adaptation of https://github.com/blazs/subsets
        std::vector<size_t> combination(w);
        int i, j, r;

        for (i = 0; i < (int) w; ++i) combination[i] = i; // Initial combination
        while (true) {
            std::vector<size_t> combinationCopy = combination;
            co_yield combinationCopy;

            if (combination[0] == n - w) break;

            for (i = w - 1; i >= 0 && combination[i] + w - i == n; --i);
            r = combination[i];
            ++combination[i];
            j = 2;
            for (++i; i < (int) w; ++i, ++j) combination[i] = r + j;
        }
    }
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperposedGameState<Realisation>::SuperposedGameState(GameStateId classicalGameState, SuperposedGameStateId id) : realisations({classicalGameState}), id(id) {
}

template<typename Realisation>
SuperposedGameState<Realisation>::SuperposedGameState(std::set<GameStateId> realisations, SuperposedGameStateId id) : realisations(realisations), id(id) {
    if (realisations.empty()) throw(std::domain_error("Cannot construct empty superposed game state."));
}

template<typename Realisation>
std::vector<typename Realisation::Piece> SuperposedGameState<Realisation>::getPieces(Player player) const {
    std::set<typename Realisation::Piece> pieces;
    for (GameStateId realisationId : realisations) {
        for (typename Realisation::Piece piece : GameStateDatabase<Realisation>::getInstance().getGameState(realisationId).getPieces(player)) {
            pieces.insert(piece);
        }
    }
    return std::vector<typename Realisation::Piece>(pieces.begin(), pieces.end());
}

#endif // SUPERPOSED_GAME_STATE_H
