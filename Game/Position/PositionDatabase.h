#ifndef POSITION_DATABASE_H
#define POSITION_DATABASE_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "Position.h"

// TODO: move into templated generic Database class
/**
 * Singleton database for keeping track of the positions we have already encountered in our computations.
 * Allows for cached results to be reused, instead of having to compute them again.
 */
template<typename PositionType>
class PositionDatabase {
public:
    static PositionDatabase<PositionType>& getInstance() { return instance; };
    PositionDatabase(const PositionDatabase& other) = delete;

    /**
     * Gets the id corresponding to the position from the database.
     * Inserts the position if it did not yet exist in the database.
     */
    PositionId getPositionId(const PositionType& position);
    /**
     * Gets the position with the given id from the database.
     */
    PositionType& getPosition(PositionId id);

private:
    PositionDatabase();
    std::vector<std::unique_ptr<PositionType>> existingPositions;
    std::unordered_map<PositionType, PositionId> positionToId;

    inline static PositionDatabase<PositionType> instance = PositionDatabase<PositionType>();
};

// This is a templated class, so the implementations need to go here

template<typename PositionType>
PositionDatabase<PositionType>::PositionDatabase() {
    existingPositions.reserve(1024);
}

template<typename PositionType>
PositionId PositionDatabase<PositionType>::getPositionId(const PositionType& position) {
    if (positionToId.contains(position)) return positionToId[position];

    PositionId id = existingPositions.size();
    positionToId[position] = id;
    existingPositions.push_back(std::make_unique<PositionType>(position));
    return id;
}

template<typename PositionType>
PositionType& PositionDatabase<PositionType>::getPosition(PositionId id) {
    return *existingPositions[id];
}

#endif // POSITION_DATABASE_H
