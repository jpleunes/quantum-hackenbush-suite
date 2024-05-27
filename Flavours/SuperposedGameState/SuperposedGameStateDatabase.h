#ifndef SUPERPOSED_GAME_STATE_DATABASE_H
#define SUPERPOSED_GAME_STATE_DATABASE_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "SuperposedGameState.h"

namespace std {
    template<>
    struct hash<std::set<GameStateId>> {
        size_t operator()(const std::set<GameStateId> realisations) const {
            std::vector<uint32_t> intVec;
            for (GameStateId realisationId : realisations) intVec.push_back((uint32_t) realisationId);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }
    };
}

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
    Flavour& getOrInsert(GameStateId classicalGameState);
    /**
     * Gets the superposed game state with the given realisations from the database.
     * Inserts the superposed game state if it did not yet exist in the database.
     */
    Flavour& getOrInsert(const std::set<GameStateId> realisations);
    /**
     * Gets the superposed game state with the given id from the database.
     */
    Flavour& getSuperposedGameState(SuperposedGameStateId id) { return *existingSuperposedGameStates[id]; }
private:
    SuperposedGameStateDatabase();
    std::vector<std::unique_ptr<Flavour>> existingSuperposedGameStates;
    std::unordered_map<std::set<GameStateId>, SuperposedGameStateId> realisationsToId;

    inline static SuperposedGameStateDatabase<Flavour> instance = SuperposedGameStateDatabase<Flavour>();
};

// This is a templated class, so the implementations need to go here

template<typename Flavour>
SuperposedGameStateDatabase<Flavour>::SuperposedGameStateDatabase() {
    existingSuperposedGameStates.reserve(1024);
}

template<typename Flavour>
Flavour& SuperposedGameStateDatabase<Flavour>::getOrInsert(GameStateId classicalGameState) {
    return getOrInsert(std::set<GameStateId>{ classicalGameState });
}

template<typename Flavour>
Flavour& SuperposedGameStateDatabase<Flavour>::getOrInsert(const std::set<GameStateId> realisations) {
    if (realisationsToId.contains(realisations)) return getSuperposedGameState(realisationsToId[realisations]);

    SuperposedGameStateId id = existingSuperposedGameStates.size();
    realisationsToId[realisations] = id;
    existingSuperposedGameStates.push_back(std::make_unique<Flavour>(realisations, id));
    return getSuperposedGameState(id);
}

#endif // SUPERPOSED_GAME_STATE_DATABASE_H
