#ifndef CREATE_POSITION_UTIL_H
#define CREATE_POSITION_UTIL_H

#include "../Flavours/SuperposedGameState/GameState/Position/AdjacencyMatrixPosition.h"
#include "../Flavours/SuperposedGameState/GameState/Position/ShortHollyhocksPosition.h"
#include "../Flavours/SuperposedGameState/GameState/Position/HackenbushStringPosition.h"

// Creates a Short Hollyhocks position, represented as an adjacency matrix.
const AdjacencyMatrixPosition createShortHollyhocksAdjacencyMatrixPosition(size_t nBlueHalves, size_t nRedHalves, size_t nBlueWholes, size_t nRedWholes);

// Creates a Short Hollyhocks position, represented as a list of halves and wholes.
const ShortHollyhocksPosition createShortHollyhocksPosition(size_t nBlueHalves, size_t nRedHalves, size_t nBlueWholes, size_t nRedWholes);

// Creates a circus tent position, represented as an adjacency matrix.
const AdjacencyMatrixPosition createCircusTentAdjacencyMatrixPosition(size_t nLegs);

// Creates all Hackenbush string positions of a given length, represented as lists of piece colours
const std::vector<HackenbushStringPosition> createHackenbushStringPositions(size_t length);

#endif // CREATE_POSITION_UTIL_H
