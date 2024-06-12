#ifndef SUPERPOSED_GAME_STATE_B_H
#define SUPERPOSED_GAME_STATE_B_H

#include "SuperposedGameState/SuperposedGameState.h"

template<typename Realisation>
class SuperposedGameStateB : public SuperposedGameState<Realisation> {
public:
    SuperposedGameStateB(GameStateId classicalGameState, SuperposedGameStateId id);
    SuperposedGameStateB(std::set<GameStateId> realisations, SuperposedGameStateId id);
    std::vector<SuperposedGameStateId> getOptions(Player player, size_t width) const override;

    ~SuperposedGameStateB() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperposedGameStateB<Realisation>::SuperposedGameStateB(GameStateId classicalGameState, SuperposedGameStateId id) : SuperposedGameState<Realisation>(classicalGameState, id) {
}

template<typename Realisation>
SuperposedGameStateB<Realisation>::SuperposedGameStateB(std::set<GameStateId> realisations, SuperposedGameStateId id) : SuperposedGameState<Realisation>(realisations, id) {
}

// For flavour B, if we are considering Short Hollyhocks positions, we can discard covered realisations
template<>
SuperposedGameStateB<ShortHollyhocksPosition>::SuperposedGameStateB(std::set<GameStateId> realisations, SuperposedGameStateId id) : SuperposedGameState<ShortHollyhocksPosition>(determineNonCoveredRealisations(realisations), id) {   
}

template<typename Realisation>
std::vector<SuperposedGameStateId> SuperposedGameStateB<Realisation>::getOptions(Player player, size_t width) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();

    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour B: if a player has only one possible move within all realisations together, 
    // they can play it as an unsuperposed move
    std::vector<SuperposedGameStateId> result;
    if (pieces.size() == 1) {
        std::set<GameStateId> option;
        for (GameStateId realisationId : this->getRealisations()) {
            GameState<Realisation> realisation = GameStateDatabase<Realisation>::getInstance().getGameState(realisationId);
            std::optional<GameStateId> newRealisationId = realisation.applyMove(pieces[0]);
            if (newRealisationId.has_value()) option.insert(newRealisationId.value());
        }
        result.emplace_back(SuperposedGameStateDatabase<SuperposedGameStateB<Realisation>>::getInstance().getOrInsert(option).getId());

        if (player == Player::LEFT) this->cache.leftOptions = result;
        else if (player == Player::RIGHT) this->cache.rightOptions = result;

        return result;
    }
    result = getSuperposedOptions<SuperposedGameStateB<Realisation>>(player, width);

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
}

#endif // SUPERPOSED_GAME_STATE_B_H
