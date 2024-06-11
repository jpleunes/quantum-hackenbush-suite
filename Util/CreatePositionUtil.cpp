#include <stdexcept>

#include "CreatePositionUtil.h"

const AdjacencyMatrixPosition createShortHollyhocksAdjacencyMatrixPosition(size_t nBlueHalves, size_t nRedHalves, size_t nBlueWholes, size_t nRedWholes) {
    const size_t requiredNodes = 1 + 2 * nBlueHalves + 2 * nRedHalves + nBlueWholes + nRedWholes;
    AdjacencyMatrixPosition position(requiredNodes);

    size_t index = 1;
    for (size_t i = 0; i < nBlueHalves; i++) {
        position.addPiece(Edge(0, index), PieceColour::BLUE);
        position.addPiece(Edge(index, index + 1), PieceColour::RED);
        index += 2;
    }
    for (size_t i = 0; i < nRedHalves; i++) {
        position.addPiece(Edge(0, index), PieceColour::RED);
        position.addPiece(Edge(index, index + 1), PieceColour::BLUE);
        index += 2;
    }
    for (size_t i = 0; i < nBlueWholes; i++) {
        position.addPiece(Edge(0, index), PieceColour::BLUE);
        index++;
    }
    for (size_t i = 0; i < nRedWholes; i++) {
        position.addPiece(Edge(0, index), PieceColour::RED);
        index++;
    }

    return position;
}

const ShortHollyhocksPosition createShortHollyhocksPosition(size_t nBlueHalves, size_t nRedHalves, size_t nBlueWholes, size_t nRedWholes) {
    ShortHollyhocksPosition position;

    size_t index = 0;
    for (size_t i = 0; i < nBlueHalves; i++) {
        position.addPiece(ShortHollyhocksPiece(index, PieceColour::BLUE));
        position.addPiece(ShortHollyhocksPiece(index, PieceColour::RED));
        index++;
    }
    for (size_t i = 0; i < nRedHalves; i++) {
        position.addPiece(ShortHollyhocksPiece(index, PieceColour::RED));
        position.addPiece(ShortHollyhocksPiece(index, PieceColour::BLUE));
        index++;
    }
    for (size_t i = 0; i < nBlueWholes; i++) position.addPiece(ShortHollyhocksPiece(index++, PieceColour::BLUE));
    for (size_t i = 0; i < nRedWholes; i++) position.addPiece(ShortHollyhocksPiece(index++, PieceColour::RED));

    return position;
}

const AdjacencyMatrixPosition createCircusTentAdjacencyMatrixPosition(size_t nLegs) {
    const size_t requiredNodes = 2 + nLegs;
    AdjacencyMatrixPosition position(requiredNodes);

    size_t top = requiredNodes - 1; // the top node of the circus tent that all red edges connect to
    for (size_t i = 1; i <= nLegs; i++) {
        position.addPiece(Edge(0, i), PieceColour::BLUE);
        position.addPiece(Edge(i, top), PieceColour::RED);
    }

    return position;
}
