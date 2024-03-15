#ifndef GAME_INSTANCE_DATABASE_H
#define GAME_INSTANCE_DATABASE_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "QuantumHackenbush.h"

// TODO: move into templated generic Database class
/**
 * TODO
 */
template<typename Ruleset>
class GameInstanceDatabase {
public:
    static GameInstanceDatabase<Ruleset>& getInstance() { return instance; };
    GameInstanceDatabase(const GameInstanceDatabase& other) = delete;

    /**
     * Gets the id corresponding to the game instance from the database.
     * Inserts the game instance if it did not yet exist in the database.
     */
    GameInstanceId getGameInstanceId(const Ruleset& gameInstance);
    /**
     * Gets the game instance with the given id from the database.
     */
    Ruleset& getGameInstance(GameInstanceId id);
private:
    GameInstanceDatabase();
    std::vector<std::unique_ptr<Ruleset>> existingGameInstances;
    std::unordered_map<Ruleset, GameInstanceId> gameInstanceToId;

    inline static GameInstanceDatabase<Ruleset> instance = GameInstanceDatabase<Ruleset>();
};

// This is a templated class, so the implementations need to go here

template<typename Ruleset>
GameInstanceDatabase<Ruleset>::GameInstanceDatabase() {
    // existingGameInstances.emplace_back(std::make_unique<Ruleset>()); // TODO: do we want to place the empty game instance here?
    existingGameInstances.reserve(1024);
}

template<typename Ruleset>
GameInstanceId GameInstanceDatabase<Ruleset>::getGameInstanceId(const Ruleset& gameInstance) {
    if (gameInstanceToId.contains(gameInstance)) return gameInstanceToId[gameInstance];

    GameInstanceId id = existingGameInstances.size();
    gameInstanceToId[gameInstance] = id;
    existingGameInstances.push_back(std::make_unique<Ruleset>(gameInstance));
    return id;
}

template<typename Ruleset>
Ruleset& GameInstanceDatabase<Ruleset>::getGameInstance(GameInstanceId id) {
    return *existingGameInstances[id];
}

#endif // GAME_INSTANCE_DATABASE_H
