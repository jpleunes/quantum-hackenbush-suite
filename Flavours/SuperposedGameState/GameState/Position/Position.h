#ifndef POSITION_H
#define POSITION_H

#include <set>
#include <cstddef>
#include <memory>
#include <map>
#include <optional>

enum class PieceColour : char {
	BLUE = 1,
	RED = -1,
	NONE = 0,
};

enum class Player : char {
    LEFT = 1,
    RIGHT = -1,
};

typedef size_t NodeId;
struct Edge {
    NodeId from;
    NodeId to;

    friend int operator<(const Edge& lhs, const Edge& rhs) {
        return lhs.from < rhs.from || (lhs.from == rhs.from && lhs.to < rhs.to);
    }
};

typedef size_t ShortHollyhocksIndex;
struct ShortHollyhocksPiece {
    ShortHollyhocksIndex index;
    PieceColour colour;

    friend int operator<(const ShortHollyhocksPiece& lhs, const ShortHollyhocksPiece& rhs) {
        return lhs.index < rhs.index || (lhs.index == rhs.index && lhs.colour < rhs.colour);
    }
};

template<typename Piece>
class Position {
public:
    virtual bool removePiece(Piece piece) = 0;
    virtual std::set<Piece> getPieces(Player player) const = 0;
    
    virtual void printHumanReadable() const = 0;

    virtual ~Position() = default;
};

#endif // POSITION_H
