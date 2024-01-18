#ifndef POSITION_H
#define POSITION_H

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
    virtual void addPiece(Edge piece, PieceColour colour) = 0;
    virtual bool removePiece(Edge piece) = 0;
    virtual std::vector<Edge> getPieces(Player player) const = 0;
    virtual void printHumanReadable() const = 0;

    virtual ~Position() = default;
};

#endif // POSITION_H
