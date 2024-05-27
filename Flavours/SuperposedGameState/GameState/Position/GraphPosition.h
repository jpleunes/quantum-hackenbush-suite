#ifndef GRAPH_POSITION_H
#define GRAPH_POSITION_H

#include "Position.h"

class GraphPosition : public Position<Edge> {
public:
    virtual void addPiece(Edge piece, PieceColour colour) = 0;
    virtual bool removePiece(Edge piece) = 0;
    virtual std::set<Edge> getPieces(Player player) const = 0;    
    virtual void printHumanReadable() const = 0;

    virtual ~GraphPosition() = default;
};

#endif // GRAPH_POSITION_H
