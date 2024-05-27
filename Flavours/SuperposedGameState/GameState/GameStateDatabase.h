#ifndef GAME_STATE_DATABASE_H
#define GAME_STATE_DATABASE_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "GameStateUtil.h"

/**
 * Singleton database for keeping track of the game states we have already encountered in our computations.
 * Allows for cached results to be reused, instead of having to compute them again.
 * Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
 */
template<typename Position>
class GameStateDatabase {
public:
    static GameStateDatabase<Position>& getInstance() { return instance; };
    GameStateDatabase(const GameStateDatabase& other) = delete;

    /**
     * Gets the GameState for the given position from the database.
     * Inserts the GameState if it did not yet exist in the database.
     */
    GameState<Position>& getOrInsert(const Position position);
    /**
     * Gets the GameState with the given id from the database.
     */
    GameState<Position>& getGameState(GameStateId id) { return *existingGameStates[id]; }

private:
    GameStateDatabase();
    std::vector<std::unique_ptr<GameState<Position>>> existingGameStates;
    std::unordered_map<Position, GameStateId> positionToId;

    inline static GameStateDatabase<Position> instance = GameStateDatabase<Position>();
};

// This is a templated class, so the implementations need to go here

template<typename Position>
GameStateDatabase<Position>::GameStateDatabase() {
    existingGameStates.reserve(1024);
}

template<typename Position>
GameState<Position>& GameStateDatabase<Position>::getOrInsert(const Position position) {
    if (positionToId.contains(position)) return getGameState(positionToId[position]);

    GameStateId id = existingGameStates.size();
    positionToId[position] = id;
    existingGameStates.push_back(std::make_unique<GameState<Position>>(position, id));
    return getGameState(id);
}

// Prevent recursive dependence
#include "GameState.h"

#endif // GAME_STATE_DATABASE_H
