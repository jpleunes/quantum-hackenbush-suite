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
template<typename Flavour>
class GameInstanceDatabase {
public:
    static GameInstanceDatabase<Flavour>& getInstance() { return instance; };
    GameInstanceDatabase(const GameInstanceDatabase& other) = delete;

    /**
     * Gets the id corresponding to the game instance from the database.
     * Inserts the game instance if it did not yet exist in the database.
     */
    GameInstanceId getGameInstanceId(const Flavour& gameInstance);
    /**
     * Gets the game instance with the given id from the database.
     */
    Flavour& getGameInstance(GameInstanceId id);
private:
    GameInstanceDatabase();
    std::vector<std::unique_ptr<Flavour>> existingGameInstances;
    std::unordered_map<Flavour, GameInstanceId> gameInstanceToId;

    inline static GameInstanceDatabase<Flavour> instance = GameInstanceDatabase<Flavour>();
};

// This is a templated class, so the implementations need to go here

template<typename Flavour>
GameInstanceDatabase<Flavour>::GameInstanceDatabase() {
    // existingGameInstances.emplace_back(std::make_unique<Flavour>()); // TODO: do we want to place the empty game instance here?
    existingGameInstances.reserve(1024);
}

template<typename Flavour>
GameInstanceId GameInstanceDatabase<Flavour>::getGameInstanceId(const Flavour& gameInstance) {
    if (gameInstanceToId.contains(gameInstance)) return gameInstanceToId[gameInstance];

    GameInstanceId id = existingGameInstances.size();
    gameInstanceToId[gameInstance] = id;
    existingGameInstances.push_back(std::make_unique<Flavour>(gameInstance));
    return id;
}

template<typename Flavour>
Flavour& GameInstanceDatabase<Flavour>::getGameInstance(GameInstanceId id) {
    return *existingGameInstances[id];
}

#endif // GAME_INSTANCE_DATABASE_H
