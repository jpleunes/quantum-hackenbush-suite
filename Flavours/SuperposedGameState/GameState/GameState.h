#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <set>

#include "Position/Position.h"
#include "GameStateDatabase.h"

template<typename Piece>
struct GameStateCache {
    std::optional<std::set<Piece>> leftPieces, rightPieces;
    std::map<Piece, std::optional<GameStateId>> moveOptions;
};

template<typename Position>
class GameState {
public:
    // This constructor should only be used from within GameStateDatabase.
    // Otherwise, GameStateDatabase.getOrInsert should be used.
    GameState(Position position, GameStateId id);

    const Position& getGameState() const { return position; }
    GameStateId getId() const { return id; }
    // Checks whether two GameState instances have the same position.
    bool operator==(const GameState<Position>& other) const { return position == other.position; }

    std::set<typename Position::Piece> getPieces(Player player) const;
    /// @brief Constructs a GameState instance representing the result of applying the given move.
    /// @param piece the piece to remove
    /// @return the id of the resulting GameState instance, or {} if the move was illegal
    std::optional<GameStateId> applyMove(typename Position::Piece piece) const;

    ~GameState() = default;

private:
    const Position position;
    const GameStateId id;

    mutable GameStateCache<typename Position::Piece> cache;
};

// This is a templated class, so the implementations need to go here

template<typename Position>
GameState<Position>::GameState(Position position, GameStateId id) : position(position), id(id) {
}

template<typename Position>
std::set<typename Position::Piece> GameState<Position>::getPieces(Player player) const {
    if (player == Player::LEFT && cache.leftPieces.has_value()) return cache.leftPieces.value();
    else if (player == Player::LEFT && cache.rightPieces.has_value()) return cache.rightPieces.value();
    
    std::set<typename Position::Piece> result = position.getPieces(player);

    if (player == Player::LEFT) cache.leftPieces = result;
    else if (player == Player::RIGHT) cache.rightPieces = result;

    return result;
}

template<typename Position>
std::optional<GameStateId> GameState<Position>::applyMove(typename Position::Piece piece) const {
    if (cache.moveOptions.contains(piece)) return cache.moveOptions[piece];

    Position resultPosition = position;
    if (!resultPosition.removePiece(piece)) {
        cache.moveOptions[piece] = {};

        return {};
    }
    GameStateId result = GameStateDatabase<Position>::getInstance().getOrInsert(resultPosition).getId();

    cache.moveOptions[piece] = result;

    return result;
}

#endif // GAME_STATE_H
