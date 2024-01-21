#ifndef SUPERPOSITION_H
#define SUPERPOSITION_H

#include <vector>
#include <set>

#include "../Position/AdjacencyMatrixPosition.h"

template<typename Piece>
class Superposition {
public:
    Superposition() = default;
    Superposition(const Position<Piece> *classicalPosition);
    size_t getWidth() const;
    bool empty() const;
    void addRealisation(const Position<Piece> *realisation);
    const Position<Piece>& getRealisation(size_t index) const;
    // Gets the pieces which exist in at least one realisation (and can thus be used in a move) 
    // for a given player.
    std::vector<Piece> getPieces(Player player) const;

    ~Superposition();
    
private:
    std::vector<const Position<Piece>*> realisations;
};

// This is a templated class, so the implementations need to go here

template<typename Piece>
Superposition<Piece>::Superposition(const Position<Piece> *classicalPosition) : realisations({classicalPosition}) {
}

template<typename Piece>
size_t Superposition<Piece>::getWidth() const {
    return realisations.size();
}

template<typename Piece>
bool Superposition<Piece>::empty() const {
    return realisations.empty();
}

template<typename Piece>
void Superposition<Piece>::addRealisation(const Position<Piece>* realisation) {
    realisations.push_back(realisation);
}

template<typename Piece>
const Position<Piece>& Superposition<Piece>::getRealisation(size_t index) const {
    return *(realisations[index]);
}

template<typename Piece>
std::vector<Piece> Superposition<Piece>::getPieces(Player player) const {
    std::set<Piece> pieces;
    for (const Position<Piece> *realisation : realisations) {
        for (Piece piece : realisation->getPieces(player)) pieces.insert(piece);
    }
    return std::vector<Piece>(pieces.begin(), pieces.end());
}

template<typename Piece>
Superposition<Piece>::~Superposition() {
    for (auto realisation : realisations) delete realisation;
}

#endif // SUPERPOSITION_H
