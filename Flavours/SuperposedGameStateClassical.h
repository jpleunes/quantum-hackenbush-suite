#ifndef SUPERPOSED_GAME_STATE_CLASSICAL_H
#define SUPERPOSED_GAME_STATE_CLASSICAL_H

#include "SuperposedGameState/SuperposedGameState.h"

template<typename Realisation>
class SuperposedGameStateClassical : public SuperposedGameState<Realisation> {
public:
    SuperposedGameStateClassical(GameStateId classicalGameState, SuperposedGameStateId id, size_t depth);
    SuperposedGameStateClassical(std::set<GameStateId> realisations, SuperposedGameStateId id, size_t depth);
    std::vector<SuperposedGameStateId> getOptions(Player player, size_t width) const override;

    ~SuperposedGameStateClassical() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperposedGameStateClassical<Realisation>::SuperposedGameStateClassical(GameStateId classicalGameState, SuperposedGameStateId id, size_t depth) : SuperposedGameState<Realisation>(classicalGameState, id, depth) {
}

template<typename Realisation>
SuperposedGameStateClassical<Realisation>::SuperposedGameStateClassical(std::set<GameStateId> realisations, SuperposedGameStateId id, size_t depth) : SuperposedGameState<Realisation>(realisations, id, depth) {
    if (realisations.size() > 1) throw(std::domain_error("Superposed game state of classical flavour should not contain more than one realisation."));
}

template<typename Realisation>
std::vector<SuperposedGameStateId> SuperposedGameStateClassical<Realisation>::getOptions(Player player, __attribute__((unused)) size_t width) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();

    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    std::vector<SuperposedGameStateId> result;
    for (size_t i = 0; i < pieces.size(); i++) {
        GameState<Realisation>& gameState = GameStateDatabase<Realisation>::getInstance().getGameState(*this->getRealisations().begin());
        GameStateId newGameStateId = gameState.applyMove(pieces[i]).value();
        SuperposedGameStateId superposedGameStateId = SuperposedGameStateDatabase<SuperposedGameStateClassical<Realisation>>::getInstance().getOrInsert(newGameStateId, this->getDepth() + 1).getId();
        result.emplace_back(superposedGameStateId);
    }

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
}

#endif // SUPERPOSED_GAME_STATE_CLASSICAL_H
