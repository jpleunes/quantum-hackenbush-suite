#ifndef POSITION_H
#define POSITION_H

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
    // Gets the pieces for a given player across all realisations.
    std::vector<Edge> getPieces(Player player) const;

    ~Superposition();
    
private:
    std::vector<const Position*> realisations;
    // We use an adjacency matrix position to represent which pieces exist in at least one 
    // realisation of our superposition (and can thus be used in a move).
    // In some cases the AdjacencyMatrixPostion will remove pieces which are no longer connected 
    // to the ground. 
    // This is okay, because if a piece does not exist in any realisation, then nothing can exist 
    // above that piece in any realisation either.
    AdjacencyMatrixPosition *possiblePieces = nullptr;

    void addPossiblePieces(const Position* realisation);
};

#endif // POSITION_H
