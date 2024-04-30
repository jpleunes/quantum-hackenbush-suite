#ifndef GAME_INSTANCE_DATABASE_H
#define GAME_INSTANCE_DATABASE_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "Superposition.h"

namespace std {
    template<>
    struct hash<std::set<HackenbushId>> {
        size_t operator()(const std::set<HackenbushId> realisations) const {
            std::vector<uint32_t> intVec;
            for (HackenbushId realisationId : realisations) intVec.push_back((uint32_t) realisationId);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }
    };
}

/**
 * A database for keeping track of all unique superpositions of some flavour that we have
 * encountered so far. Superpositions are considered to be the same if they have label-isomorphic
 * realisations.
 * Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
 */
template<typename Flavour>
class SuperpositionDatabase {
public:
    static SuperpositionDatabase<Flavour>& getInstance() { return instance; };
    SuperpositionDatabase(const SuperpositionDatabase& other) = delete;

    /**
     * Gets the superposition corresponding to the given classical position from the database.
     * Inserts the superposition if it did not yet exist in the database.
     */
    Flavour& getOrInsert(HackenbushId classicalPosition);
    /**
     * Gets the superposition with the given realisations from the database.
     * Inserts the superposition if it did not yet exist in the database.
     */
    Flavour& getOrInsert(const std::set<HackenbushId> realisations);
    /**
     * Gets the superposition with the given id from the database.
     */
    Flavour& getSuperposition(SuperpositionId id) { return *existingSuperpositions[id]; }
private:
    SuperpositionDatabase();
    std::vector<std::unique_ptr<Flavour>> existingSuperpositions;
    std::unordered_map<std::set<HackenbushId>, SuperpositionId> realisationsToId;

    inline static SuperpositionDatabase<Flavour> instance = SuperpositionDatabase<Flavour>();
};

// This is a templated class, so the implementations need to go here

template<typename Flavour>
SuperpositionDatabase<Flavour>::SuperpositionDatabase() {
    existingSuperpositions.reserve(1024);
}

template<typename Flavour>
Flavour& SuperpositionDatabase<Flavour>::getOrInsert(HackenbushId classicalPosition) {
    return getOrInsert(std::set<HackenbushId>{ classicalPosition });
}

template<typename Flavour>
Flavour& SuperpositionDatabase<Flavour>::getOrInsert(const std::set<HackenbushId> realisations) {
    if (realisationsToId.contains(realisations)) return getSuperposition(realisationsToId[realisations]);

    SuperpositionId id = existingSuperpositions.size();
    realisationsToId[realisations] = id;
    existingSuperpositions.push_back(std::make_unique<Flavour>(realisations, id));
    return getSuperposition(id);
}

#endif // GAME_INSTANCE_DATABASE_H
