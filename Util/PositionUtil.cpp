#include <stdexcept>

#include "PositionUtil.h"

const AdjacencyMatrixPosition *createRestrictedPosition(int nodeCount, int nBlueHalfs, int nRedHalfs, int nBlueWholes, int nRedWholes) {
    AdjacencyMatrixPosition *classicalPosition = new AdjacencyMatrixPosition(nodeCount);

    const int requiredNodes = 1 + 2 * nBlueHalfs + 2 * nRedHalfs + nBlueWholes + nRedWholes;
    if (requiredNodes > nodeCount) {
        throw std::domain_error("The number of nodes " + std::to_string(nodeCount) + " is too small for the required number " + std::to_string(requiredNodes) + " for the specified pieces.");
    }
    int index = 1;
    for (int i = 0; i < nBlueHalfs; i++) {
        classicalPosition->addPiece(Edge(0, index), PieceColour::BLUE);
        classicalPosition->addPiece(Edge(index, index + 1), PieceColour::RED);
        index += 2;
    }
    for (int i = 0; i < nRedHalfs; i++) {
        classicalPosition->addPiece(Edge(0, index), PieceColour::RED);
        classicalPosition->addPiece(Edge(index, index + 1), PieceColour::BLUE);
        index += 2;
    }
    for (int i = 0; i < nBlueWholes; i++) {
        classicalPosition->addPiece(Edge(0, index), PieceColour::BLUE);
        index++;
    }
    for (int i = 0; i < nRedWholes; i++) {
        classicalPosition->addPiece(Edge(0, index), PieceColour::RED);
        index++;
    }

    return classicalPosition;
}
