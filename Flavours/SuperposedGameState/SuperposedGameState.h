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

const int width = 2;

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

    virtual std::vector<SuperposedGameStateId> getOptions(Player player) const = 0;

    // Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
    template<typename Flavour>
    ShortGameId determineShortGameId() const {
        if (cache.shortGameId.has_value()) return cache.shortGameId.value();

        std::set<ShortGameId> leftOptions, rightOptions;
        for (SuperposedGameStateId leftOption : getOptions(Player::LEFT)) {
            leftOptions.insert(SuperposedGameStateDatabase<Flavour>::getInstance().getSuperposedGameState(leftOption).template determineShortGameId<Flavour>());
        }
        for (SuperposedGameStateId rightOption : getOptions(Player::RIGHT)) {
            rightOptions.insert(SuperposedGameStateDatabase<Flavour>::getInstance().getSuperposedGameState(rightOption).template determineShortGameId<Flavour>());
        }
        ShortGameId result = ShortGameDatabase::getInstance().getOrInsert(leftOptions, rightOptions).getId();

        cache.shortGameId = result;

        return result;
    }

    virtual ~SuperposedGameState() = default;

protected:
    template<typename Flavour>
    std::vector<SuperposedGameStateId> getSuperposedOptions(Player player) const { // TODO: also allow moves with width >2 (use width as the maximum width?)
        std::vector<typename Realisation::Piece> pieces = getPieces(player);

        std::vector<SuperposedGameStateId> result;
        if (pieces.size() < width) return result;
        auto indexCombinationsGen = indexCombinations(pieces.size());
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
        return result;
    }

    mutable SuperposedGameStateCache cache;
    
private:
    const std::set<GameStateId> realisations;
    const SuperposedGameStateId id;

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
