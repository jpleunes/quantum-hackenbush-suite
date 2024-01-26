#include "../Game/Position/AdjacencyMatrixPosition.h"
#include "../Game/Position/RestrictedPosition.h"

// Creates a 1-wide starting superposition for the restricted variant of Hackenbush,
// represented as an adjacency matrix.
const AdjacencyMatrixPosition* createRestrictedAdjacencyMatrixPosition(size_t nBlueHalfs, size_t nRedHalfs, size_t nBlueWholes, size_t nRedWholes);

// Creates a 1-wide starting superposition for the restricted variant of Hackenbush,
// represented as a list of halves and wholes.
const RestrictedPosition* createRestrictedPosition(size_t nBlueHalfs, size_t nRedHalfs, size_t nBlueWholes, size_t nRedWholes);
