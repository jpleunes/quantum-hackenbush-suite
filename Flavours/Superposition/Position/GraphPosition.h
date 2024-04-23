#ifndef GRAPH_POSITION_H
#define GRAPH_POSITION_H

#include "Position.h"

class GraphPosition : public Position<Edge> {
public:
    virtual void addPiece(Edge piece, PieceColour colour) = 0;
    virtual std::vector<Edge> getPieces(Player player) const = 0;    
    /// @brief Constructs a new Position representing the result of applying the given move.
    /// @param piece the piece to remove
    /// @return the id of the resulting Position, or ILLEGAL_POSITION_ID if the move was illegal
    virtual PositionId applyMove(Edge piece) const = 0;
    virtual void printHumanReadable() const = 0;

    virtual ~GraphPosition() = default;
};

#endif // GRAPH_POSITION_H
