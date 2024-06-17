#ifndef SUPERPOSED_GAME_STATE_DATABASE_H
#define SUPERPOSED_GAME_STATE_DATABASE_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "SuperposedGameState.h"
#include "GameState/Position/AdjacencyMatrixPosition.h"
#include "GameState/Position/ShortHollyhocksPosition.h"
#include "Realisations.h"

/**
 * A database for keeping track of all unique superposed game states of some flavour that we have
 * encountered so far. Superposed game states are considered to be the equal if they have equal realisations.
 * Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
 */
template<typename Flavour>
class SuperposedGameStateDatabase {
public:
    static SuperposedGameStateDatabase<Flavour>& getInstance() { return instance; };
    SuperposedGameStateDatabase(const SuperposedGameStateDatabase& other) = delete;

    /**
     * Gets the superposed game state corresponding to the given classical game state from the database.
     * Inserts the superposed game state if it did not yet exist in the database.
     */
    Flavour& getOrInsert(GameStateId classicalGameState, size_t depth);
    /**
     * Gets the superposed game state with the given realisations from the database.
     * Inserts the superposed game state if it did not yet exist in the database.
     */
    Flavour& getOrInsert(const std::set<GameStateId> realisations, size_t depth);
    /**
     * Gets the superposed game state with the given id from the database.
     */
    Flavour& getSuperposedGameState(SuperposedGameStateId id) { return *existingSuperposedGameStates[id]; }
private:
    SuperposedGameStateDatabase();
    std::vector<std::unique_ptr<Flavour>> existingSuperposedGameStates;
    std::unordered_map<typename Flavour::RealisationType::Position::RealisationsType, SuperposedGameStateId> realisationsToId;

    inline static SuperposedGameStateDatabase<Flavour> instance = SuperposedGameStateDatabase<Flavour>();
};

// This is a templated class, so the implementations need to go here

template<typename Flavour>
SuperposedGameStateDatabase<Flavour>::SuperposedGameStateDatabase() {
    existingSuperposedGameStates.reserve(1024);
}

template<typename Flavour>
Flavour& SuperposedGameStateDatabase<Flavour>::getOrInsert(GameStateId classicalGameState, size_t depth) {
    return getOrInsert(std::set<GameStateId>{ classicalGameState }, depth);
}

template<typename Flavour>
Flavour& SuperposedGameStateDatabase<Flavour>::getOrInsert(const std::set<GameStateId> gameStateIds, size_t depth) {
    typename Flavour::RealisationType::Position::RealisationsType realisations(gameStateIds, depth);
    if (realisationsToId.contains(realisations)) return getSuperposedGameState(realisationsToId[realisations]);

    SuperposedGameStateId id = existingSuperposedGameStates.size();
    realisationsToId[realisations] = id;
    existingSuperposedGameStates.push_back(std::make_unique<Flavour>(gameStateIds, id, depth));
    return getSuperposedGameState(id);
}

#endif // SUPERPOSED_GAME_STATE_DATABASE_H
