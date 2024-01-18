#include <stdexcept>
#include <string>
#include <set>

#include "Superposition.h"

Superposition::Superposition() {
}

Superposition::Superposition(const Position *classicalPosition) : realisations({classicalPosition}) {
}

size_t Superposition::getWidth() const {
    return realisations.size();
}

bool Superposition::empty() const {
    return realisations.empty();
}

void Superposition::addRealisation(const Position* realisation) {
    realisations.push_back(realisation);
}

const Position& Superposition::getRealisation(size_t index) const {
    return *(realisations[index]);
}

std::vector<Edge> Superposition::getPieces(Player player) const {
    std::set<Edge> pieces;
    for (const Position *realisation : realisations) {
        for (Edge piece : realisation->getPieces(player)) pieces.insert(piece);
    }
    return std::vector<Edge>(pieces.begin(), pieces.end());
}

Superposition::~Superposition() {
    for (auto realisation : realisations) delete realisation;
}
