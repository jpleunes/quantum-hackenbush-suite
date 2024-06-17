#ifndef SUPERPOSED_GAME_STATE_A_H
#define SUPERPOSED_GAME_STATE_A_H

#include "SuperposedGameState/SuperposedGameState.h"

template<typename Realisation>
class SuperposedGameStateA : public SuperposedGameState<Realisation> {
public:
    SuperposedGameStateA(GameStateId classicalGameState, SuperposedGameStateId id, size_t depth);
    SuperposedGameStateA(std::set<GameStateId> realisations, SuperposedGameStateId id, size_t depth);
    std::vector<SuperposedGameStateId> getOptions(Player player, size_t width) const override;

    ~SuperposedGameStateA() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperposedGameStateA<Realisation>::SuperposedGameStateA(GameStateId classicalGameState, SuperposedGameStateId id, size_t depth) : SuperposedGameState<Realisation>(classicalGameState, id, depth) {
}

template<typename Realisation>
SuperposedGameStateA<Realisation>::SuperposedGameStateA(std::set<GameStateId> realisations, SuperposedGameStateId id, size_t depth) : SuperposedGameState<Realisation>(realisations, id, depth) {
}

// For flavour A, if we are considering Short Hollyhocks positions, we can discard covered realisations
template<>
SuperposedGameStateA<ShortHollyhocksPosition>::SuperposedGameStateA(std::set<GameStateId> realisations, SuperposedGameStateId id, size_t depth) : SuperposedGameState<ShortHollyhocksPosition>(determineNonCoveredRealisations(realisations), id, depth) {   
}

template<typename Realisation>
std::vector<SuperposedGameStateId> SuperposedGameStateA<Realisation>::getOptions(Player player, size_t width) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();
    
    std::vector<SuperposedGameStateId> result = getSuperposedOptions<SuperposedGameStateA<Realisation>>(player, width);

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
}

#endif // SUPERPOSED_GAME_STATE_A_H
