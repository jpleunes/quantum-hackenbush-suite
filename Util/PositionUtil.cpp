#include <stdexcept>

#include "PositionUtil.h"

const AdjacencyMatrixPosition* createRestrictedAdjacencyMatrixPosition(size_t nBlueHalfs, size_t nRedHalfs, size_t nBlueWholes, size_t nRedWholes) {
    const size_t requiredNodes = 1 + 2 * nBlueHalfs + 2 * nRedHalfs + nBlueWholes + nRedWholes;
    AdjacencyMatrixPosition *position = new AdjacencyMatrixPosition(requiredNodes);

    size_t index = 1;
    for (size_t i = 0; i < nBlueHalfs; i++) {
        position->addPiece(Edge(0, index), PieceColour::BLUE);
        position->addPiece(Edge(index, index + 1), PieceColour::RED);
        index += 2;
    }
    for (size_t i = 0; i < nRedHalfs; i++) {
        position->addPiece(Edge(0, index), PieceColour::RED);
        position->addPiece(Edge(index, index + 1), PieceColour::BLUE);
        index += 2;
    }
    for (size_t i = 0; i < nBlueWholes; i++) {
        position->addPiece(Edge(0, index), PieceColour::BLUE);
        index++;
    }
    for (size_t i = 0; i < nRedWholes; i++) {
        position->addPiece(Edge(0, index), PieceColour::RED);
        index++;
    }

    return position;
}

const RestrictedPosition* createRestrictedPosition(size_t nBlueHalfs, size_t nRedHalfs, size_t nBlueWholes, size_t nRedWholes) {
    RestrictedPosition *position = new RestrictedPosition();

    size_t index = 0;
    for (size_t i = 0; i < nBlueHalfs; i++) {
        position->addPiece(RestrictedPiece(index, PieceColour::BLUE));
        position->addPiece(RestrictedPiece(index, PieceColour::RED));
        index++;
    }
    for (size_t i = 0; i < nRedHalfs; i++) {
        position->addPiece(RestrictedPiece(index, PieceColour::RED));
        position->addPiece(RestrictedPiece(index, PieceColour::BLUE));
        index++;
    }
    for (size_t i = 0; i < nBlueWholes; i++) position->addPiece(RestrictedPiece(index++, PieceColour::BLUE));
    for (size_t i = 0; i < nRedWholes; i++) position->addPiece(RestrictedPiece(index++, PieceColour::RED));

    return position;
}
