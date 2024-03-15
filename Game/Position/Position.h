#ifndef POSITION_H
#define POSITION_H

#include <vector>
#include <cstddef>
#include <memory>
#include <map>
#include <optional>
#include <limits>

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

typedef size_t RestrictedIndex;
struct RestrictedPiece {
    RestrictedIndex index;
    PieceColour colour;

    friend int operator<(const RestrictedPiece& lhs, const RestrictedPiece& rhs) {
        return lhs.index < rhs.index || (lhs.index == rhs.index && lhs.colour < rhs.colour);
    }
};

typedef size_t PositionId;
#define ILLEGAL_POSITION_ID std::numeric_limits<PositionId>::max()

template<typename Piece>
struct PositionCacheBlock {
    std::optional<std::vector<Piece>> leftPieces;
    std::optional<std::vector<Piece>> rightPieces;
    std::map<Piece, PositionId> moveOptions;
};

template<typename Piece>
class Position {
public:
    virtual std::vector<Piece> getPieces(Player player) const = 0;    
    /// @brief Constructs a new Position representing the result of applying the given move.
    /// @param piece the piece to remove
    /// @return the resulting Position, or nullptr if the move was illegal
    virtual PositionId applyMove(Piece piece) const = 0;
    virtual void printHumanReadable() const = 0;

    virtual ~Position() = default;

protected:
    mutable PositionCacheBlock<Piece> cache;
};

#endif // POSITION_H
