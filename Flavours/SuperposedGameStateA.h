#ifndef SUPERPOSED_GAME_STATE_A_H
#define SUPERPOSED_GAME_STATE_A_H

#include "SuperposedGameState/SuperposedGameState.h"

template<typename Realisation>
class SuperposedGameStateA : public SuperposedGameState<Realisation> {
public:
    SuperposedGameStateA(GameStateId classicalGameState, SuperposedGameStateId id);
    SuperposedGameStateA(std::set<GameStateId> realisations, SuperposedGameStateId id);
    std::vector<SuperposedGameStateId> getOptions(Player player) const override;

    ~SuperposedGameStateA() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperposedGameStateA<Realisation>::SuperposedGameStateA(GameStateId classicalGameState, SuperposedGameStateId id) : SuperposedGameState<Realisation>(classicalGameState, id) {
};

template<typename Realisation>
SuperposedGameStateA<Realisation>::SuperposedGameStateA(std::set<GameStateId> realisations, SuperposedGameStateId id) : SuperposedGameState<Realisation>(realisations, id) {
}

template<typename Realisation>
std::vector<SuperposedGameStateId> SuperposedGameStateA<Realisation>::getOptions(Player player) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();
    
    std::vector<SuperposedGameStateId> result = getSuperposedOptions<SuperposedGameStateA<Realisation>>(player);

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
};

#endif // SUPERPOSED_GAME_STATE_A_H
