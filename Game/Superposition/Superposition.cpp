#include <stdexcept>
#include <string>

#include "Superposition.h"

void Superposition::addPossiblePieces(const Position* realisation) {
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

Superposition::Superposition() {
    possiblePieces = nullptr;
}

Superposition::Superposition(const Position *classicalPosition) : realisations({classicalPosition}) {
    addPossiblePieces(classicalPosition);
}

size_t Superposition::getWidth() const {
    return realisations.size();
}

bool Superposition::empty() const {
    return realisations.empty();
}

void Superposition::addRealisation(const Position* realisation) {
    realisations.push_back(realisation);

    addPossiblePieces(realisation);
}

const Position& Superposition::getRealisation(size_t index) const {
    return *(realisations[index]);
}

// Gets the pieces for a given player across all realisations.
std::vector<Edge> Superposition::getPieces(Player player) const {
    return possiblePieces->getPieces(player);
}

Superposition::~Superposition() {
    for (auto realisation : realisations) delete realisation;
    delete possiblePieces;
}
