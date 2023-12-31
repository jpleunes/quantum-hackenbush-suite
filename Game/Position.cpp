#include <stdexcept>
#include <string>

#include "Position.h"

void Position::addPossiblePieces(const ClassicalPosition* realisation) {
    if (possiblePieces == nullptr) possiblePieces = new AdjacencyMatrixPosition(realisation->getNodeCount());

    size_t nodeCount = possiblePieces->getNodeCount(); // TODO: find number of possible nodes (nodes may have been deleted in some realisations)
    for (size_t i = 0; i < nodeCount; i++) {
        for (size_t j = 0; j < i; j++) {
            PieceColour pieceColour = realisation->getPieceColour(std::make_pair(i, j));
            if (pieceColour != PieceColour::NONE) {
                // TODO: check if existing piece colour matches, error otherwise
                possiblePieces->setPieceColour(std::make_pair(i,j), pieceColour);
            }
        }
    }
}

Position::Position() {
    possiblePieces = nullptr;
}

Position::Position(const ClassicalPosition *classicalPosition) : realisations({classicalPosition}) {
    addPossiblePieces(classicalPosition);
}

size_t Position::getWidth() const {
    return realisations.size();
}

bool Position::empty() const {
    return realisations.empty();
}

void Position::addRealisation(const ClassicalPosition* realisation) {
    realisations.push_back(realisation);

    addPossiblePieces(realisation);
}

const ClassicalPosition& Position::getRealisation(size_t index) const {
    return *(realisations[index]);
}

// Gets the blue pieces across all realisations.
std::vector<Edge> Position::getBluePieces() const {
    return possiblePieces->getBluePieces();
}

// Gets the red pieces across all realisations.
std::vector<Edge> Position::getRedPieces() const {
    return possiblePieces->getRedPieces();
}

Position::~Position() {
    for (auto realisation : realisations) delete realisation;
    delete possiblePieces;
}
