#ifndef CREATE_POSITION_UTIL_H
#define CREATE_POSITION_UTIL_H

#include "../Flavours/SuperposedGameState/GameState/Position/AdjacencyMatrixPosition.h"
#include "../Flavours/SuperposedGameState/GameState/Position/RestrictedPosition.h"

// Creates a position for the restricted variant of Hackenbush, represented as an adjacency matrix.
const AdjacencyMatrixPosition createRestrictedAdjacencyMatrixPosition(size_t nBlueHalves, size_t nRedHalves, size_t nBlueWholes, size_t nRedWholes);

// Creates a position for the restricted variant of Hackenbush, represented as a list of halves and wholes.
const RestrictedPosition createRestrictedPosition(size_t nBlueHalves, size_t nRedHalves, size_t nBlueWholes, size_t nRedWholes);

// Creates a circus tent position, represented as an adjacency matrix.
const AdjacencyMatrixPosition createCircusTentAdjacencyMatrixPosition(size_t nLegs);

#endif // CREATE_POSITION_UTIL_H
