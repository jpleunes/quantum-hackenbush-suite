#ifndef SUPERPOSED_GAME_STATE_D_H
#define SUPERPOSED_GAME_STATE_D_H

#include "SuperposedGameState/SuperposedGameState.h"

template<typename Realisation>
class SuperposedGameStateD : public SuperposedGameState<Realisation> {
public:
    SuperposedGameStateD(GameStateId classcialGameState, SuperposedGameStateId id);
    SuperposedGameStateD(std::set<GameStateId> realisations, SuperposedGameStateId id);
    std::vector<SuperposedGameStateId> getOptions(Player player, size_t width) const override;

    ~SuperposedGameStateD() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperposedGameStateD<Realisation>::SuperposedGameStateD(GameStateId classcialGameState, SuperposedGameStateId id) : SuperposedGameState<Realisation>(classcialGameState, id) {
}

template<typename Realisation>
SuperposedGameStateD<Realisation>::SuperposedGameStateD(std::set<GameStateId> realisations, SuperposedGameStateId id) : SuperposedGameState<Realisation>(realisations, id) {
}

// For flavour D, if we are considering Short Hollyhocks positions, we can discard covered realisations
template<>
SuperposedGameStateD<ShortHollyhocksPosition>::SuperposedGameStateD(std::set<GameStateId> realisations, SuperposedGameStateId id) : SuperposedGameState<ShortHollyhocksPosition>(determineNonCoveredRealisations(realisations), id) {   
}

template<typename Realisation>
std::vector<SuperposedGameStateId> SuperposedGameStateD<Realisation>::getOptions(Player player, size_t width) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();

    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour D: unsuperposed moves are always allowed
    std::vector<SuperposedGameStateId> result;
    for (typename Realisation::Piece piece : pieces) {
        std::set<GameStateId> option;
        for (GameStateId realisationId : this->getRealisations()) {
            GameState<Realisation> realisation = GameStateDatabase<Realisation>::getInstance().getGameState(realisationId);
            std::optional<GameStateId> newRealisationId = realisation.applyMove(piece);
            if (newRealisationId.has_value()) option.insert(newRealisationId.value());
        }
        if (!option.empty()) result.emplace_back(SuperposedGameStateDatabase<SuperposedGameStateD<Realisation>>::getInstance().getOrInsert(option).getId());
    }
    std::vector<SuperposedGameStateId> superposedOptions = getSuperposedOptions<SuperposedGameStateD<Realisation>>(player, width);
    result.insert(result.end(), superposedOptions.begin(), superposedOptions.end());

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
}

#endif // SUPERPOSED_GAME_STATE_D_H
