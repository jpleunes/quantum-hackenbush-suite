#ifndef SUPERPOSITION_H
#define SUPERPOSITION_H

#include <vector>

#include "../Position/AdjacencyMatrixPosition.h"

class Superposition {
public:
    Superposition();
    Superposition(const Position *classicalPosition);
    size_t getWidth() const;
    bool empty() const;
    void addRealisation(const Position *realisation);
    const Position& getRealisation(size_t index) const;
    // Gets the pieces which exist in at least one realisation (and can thus be used in a move) 
    // for a given player.
    std::vector<Edge> getPieces(Player player) const;

    ~Superposition();
    
private:
    std::vector<const Position*> realisations;
};

#endif // SUPERPOSITION_H
