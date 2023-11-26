#include <stdexcept>
#include <string>

#include "Position.h"

Position::Position(const std::vector<const ClassicalPosition*>& realisations) : realisations(realisations) {
}

Position::Position(const ClassicalPosition *classicalPosition) : realisations({classicalPosition}) {
}

size_t Position::getWidth() const {
    return realisations.size();
}

bool Position::empty() const {
    return realisations.empty();
}

void Position::addRealisation(const ClassicalPosition* realization) {
    realisations.push_back(realization);
}

const ClassicalPosition& Position::getRealisation(size_t index) const {
    return *(realisations[index]);
}

// Gets the blue pieces across all realisations.
std::vector<Edge> Position::getBluePieces() const {
    std::vector<Edge> bluePieces;
    for (size_t i = 0; i < realisations.size(); i++) {
        std::vector<Edge> realizationBluePieces = realisations[i]->getBluePieces();
        bluePieces.insert(bluePieces.end(), realizationBluePieces.begin(), realizationBluePieces.end());
    }
    return bluePieces;
}

// Gets the red pieces across all realisations.
std::vector<Edge> Position::getRedPieces() const {
    std::vector<Edge> redPieces;
    for (size_t i = 0; i < realisations.size(); i++) {
        std::vector<Edge> realizationRedPieces = realisations[i]->getRedPieces();
        redPieces.insert(redPieces.end(), realizationRedPieces.begin(), realizationRedPieces.end());
    }
    return redPieces;
}

Position::~Position() {
    for (size_t i = 0; i < realisations.size(); i++) {
        delete realisations[i];
    }
}
