#ifndef GRAPH_POSITION_H
#define GRAPH_POSITION_H

#include "Position.h"
#include "../../RealisationsUtil.h"

class GraphPosition : public Position<Edge, AdjacencyMatrixRealisations> {
public:
    virtual void addPiece(Edge piece, PieceColour colour) = 0;
    virtual bool removePiece(Edge piece) = 0;
    virtual std::set<Edge> getPieces(Player player) const = 0;    
    virtual void printHumanReadable() const = 0;

    virtual ~GraphPosition() = default;
};

// Prevent recursive dependence
#include "../../Realisations.h"

#endif // GRAPH_POSITION_H
