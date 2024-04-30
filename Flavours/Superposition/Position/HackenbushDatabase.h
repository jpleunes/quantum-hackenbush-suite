#ifndef POSITION_DATABASE_H
#define POSITION_DATABASE_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "HackenbushUtil.h"

/**
 * Singleton database for keeping track of the positions we have already encountered in our computations.
 * Allows for cached results to be reused, instead of having to compute them again.
 * Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
 */
template<typename Position>
class HackenbushDatabase {
public:
    static HackenbushDatabase<Position>& getInstance() { return instance; };
    HackenbushDatabase(const HackenbushDatabase& other) = delete;

    /**
     * Gets the Hackenbush position for the given position from the database.
     * Inserts the Hackenbush position if it did not yet exist in the database.
     */
    Hackenbush<Position>& getOrInsert(const Position position);
    /**
     * Gets the Hackenbush position with the given id from the database.
     */
    Hackenbush<Position>& getPosition(HackenbushId id) { return *existingPositions[id]; }

private:
    HackenbushDatabase();
    std::vector<std::unique_ptr<Hackenbush<Position>>> existingPositions;
    std::unordered_map<Position, HackenbushId> positionToId;

    inline static HackenbushDatabase<Position> instance = HackenbushDatabase<Position>();
};

// This is a templated class, so the implementations need to go here

template<typename Position>
HackenbushDatabase<Position>::HackenbushDatabase() {
    existingPositions.reserve(1024);
}

template<typename Position>
Hackenbush<Position>& HackenbushDatabase<Position>::getOrInsert(const Position position) {
    if (positionToId.contains(position)) return getPosition(positionToId[position]);

    HackenbushId id = existingPositions.size();
    positionToId[position] = id;
    existingPositions.push_back(std::make_unique<Hackenbush<Position>>(position, id));
    return getPosition(id);
}

// Prevent recursive dependence
#include "Hackenbush.h"

#endif // POSITION_DATABASE_H
