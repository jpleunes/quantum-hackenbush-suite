#ifndef GAME_INSTANCE_DATABASE_H
#define GAME_INSTANCE_DATABASE_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "Superposition.h"

// TODO: move into templated generic Database class
/**
 * TODO
 */
template<typename Flavour>
class SuperpositionDatabase {
public:
    static SuperpositionDatabase<Flavour>& getInstance() { return instance; };
    SuperpositionDatabase(const SuperpositionDatabase& other) = delete;

    /**
     * Gets the id corresponding to the superposition from the database.
     * Inserts the superposition if it did not yet exist in the database.
     */
    SuperpositionId getSuperpositionId(const Flavour& superposition);
    /**
     * Gets the superposition with the given id from the database.
     */
    Flavour& getSuperposition(SuperpositionId id);
private:
    SuperpositionDatabase();
    std::vector<std::unique_ptr<Flavour>> existingSuperpositions;
    std::unordered_map<Flavour, SuperpositionId> superpositionToId;

    inline static SuperpositionDatabase<Flavour> instance = SuperpositionDatabase<Flavour>();
};

// This is a templated class, so the implementations need to go here

template<typename Flavour>
SuperpositionDatabase<Flavour>::SuperpositionDatabase() {
    // existingSuperpositions.emplace_back(std::make_unique<Flavour>()); // TODO: do we want to place the empty superposition here?
    existingSuperpositions.reserve(1024);
}

template<typename Flavour>
SuperpositionId SuperpositionDatabase<Flavour>::getSuperpositionId(const Flavour& superposition) {
    if (superpositionToId.contains(superposition)) return superpositionToId[superposition];

    SuperpositionId id = existingSuperpositions.size();
    superpositionToId[superposition] = id;
    existingSuperpositions.push_back(std::make_unique<Flavour>(superposition));
    return id;
}

template<typename Flavour>
Flavour& SuperpositionDatabase<Flavour>::getSuperposition(SuperpositionId id) {
    return *existingSuperpositions[id];
}

#endif // GAME_INSTANCE_DATABASE_H
