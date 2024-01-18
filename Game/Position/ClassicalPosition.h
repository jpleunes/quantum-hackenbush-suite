#ifndef CLASSICAL_POSITION_H
#define CLASSICAL_POSITION_H

#include <vector>
#include <cstddef>

typedef size_t NodeId;
typedef std::pair<NodeId, NodeId> Edge;

enum class PieceColour {
	BLUE = (char) 1,
	RED = (char) -1,
	NONE = (char) 0,
};

enum class Player {
    LEFT = (char) 1,
    RIGHT = (char) -1,
};

class Position {
public:
    virtual Position *clone() const = 0;
    virtual size_t getNodeCount() const = 0;
    virtual void increaseNodeCount(size_t count) = 0;
    virtual void addPiece(Edge piece, PieceColour colour) = 0;
    virtual bool removePiece(Edge piece) = 0;
    virtual std::vector<Edge> getPieces(Player player) const = 0;
    virtual PieceColour getPieceColour(Edge piece) const = 0;
    virtual void setPieceColour(Edge piece, PieceColour colour) = 0;
    virtual void printHumanReadable() const = 0;

    virtual ~Position() = default;
};

#endif // CLASSICAL_POSITION_H
