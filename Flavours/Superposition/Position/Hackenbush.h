#ifndef HACKENBUSH_H
#define HACKENBUSH_H

#include <set>

#include "Position.h"
#include "HackenbushDatabase.h"

template<typename Piece>
struct HackenbushCache {
    std::optional<std::set<Piece>> leftPieces, rightPieces;
    std::map<Piece, std::optional<HackenbushId>> moveOptions;
};

template<typename Position>
class Hackenbush {
public:
    // This constructor should only be used from within HackenbushDatabase.
    // Otherwise, HackenbushDatabase.getOrInsert should be used.
    Hackenbush(Position position, HackenbushId id);

    const Position& getPosition() const { return position; }
    HackenbushId getId() const { return id; }
    // Checks whether two Hackenbush instances have the same position.
    bool operator==(const Hackenbush<Position>& other) const { return position == other.position; }

    std::set<typename Position::Piece> getPieces(Player player) const;
    /// @brief Constructs a Hackenbush instance representing the result of applying the given move.
    /// @param piece the piece to remove
    /// @return the id of the resulting Hackenbush instance, or {} if the move was illegal
    std::optional<HackenbushId> applyMove(typename Position::Piece piece) const;

    ~Hackenbush() = default;

private:
    const Position position;
    const HackenbushId id;

    mutable HackenbushCache<typename Position::Piece> cache;
};

// This is a templated class, so the implementations need to go here

template<typename Position>
Hackenbush<Position>::Hackenbush(Position position, HackenbushId id) : position(position), id(id) {
}

template<typename Position>
std::set<typename Position::Piece> Hackenbush<Position>::getPieces(Player player) const {
    if (player == Player::LEFT && cache.leftPieces.has_value()) return cache.leftPieces.value();
    else if (player == Player::LEFT && cache.rightPieces.has_value()) return cache.rightPieces.value();
    
    std::set<typename Position::Piece> result = position.getPieces(player);

    if (player == Player::LEFT) cache.leftPieces = result;
    else if (player == Player::RIGHT) cache.rightPieces = result;

    return result;
}

template<typename Position>
std::optional<HackenbushId> Hackenbush<Position>::applyMove(typename Position::Piece piece) const {
    if (cache.moveOptions.contains(piece)) return cache.moveOptions[piece];

    Position resultPosition = position;
    if (!resultPosition.removePiece(piece)) {
        cache.moveOptions[piece] = {};

        return {};
    }
    HackenbushId result = HackenbushDatabase<Position>::getInstance().getOrInsert(resultPosition).getId();

    cache.moveOptions[piece] = result;

    return result;
}

#endif // HACKENBUSH_H
