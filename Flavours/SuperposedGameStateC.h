#ifndef SUPERPOSED_GAME_STATE_C_H
#define SUPERPOSED_GAME_STATE_C_H

#include "SuperposedGameState/SuperposedGameState.h"

template<typename Realisation>
class SuperposedGameStateC : public SuperposedGameState<Realisation> {
public:
    SuperposedGameStateC(GameStateId classicalGameState, SuperposedGameStateId);
    SuperposedGameStateC(std::set<GameStateId> realisations, SuperposedGameStateId id);
    std::vector<SuperposedGameStateId> getOptions(Player player, size_t width) const override;

    ~SuperposedGameStateC() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperposedGameStateC<Realisation>::SuperposedGameStateC(GameStateId classicalGameState, SuperposedGameStateId id) : SuperposedGameState<Realisation>(classicalGameState) {
};

template<typename Realisation>
SuperposedGameStateC<Realisation>::SuperposedGameStateC(std::set<GameStateId> realisations, SuperposedGameStateId id) : SuperposedGameState<Realisation>(realisations, id) {
}

template<typename Realisation>
std::vector<SuperposedGameStateId> SuperposedGameStateC<Realisation>::getOptions(Player player, size_t width) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();

    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    std::vector<SuperposedGameStateId> result;
    for (typename Realisation::Piece piece : pieces) {
        std::set<GameStateId> option;
        bool allValid = true;
        for (GameStateId realisationId : this->getRealisations()) {
            GameState<Realisation> realisation = GameStateDatabase<Realisation>::getInstance().getGameState(realisationId);
            std::optional<GameStateId> newRealisationId = realisation.applyMove(piece);
            if (newRealisationId.has_value()) option.insert(newRealisationId.value());
            else allValid = false;
        }
        if (allValid && !option.empty()) result.emplace_back(SuperposedGameStateDatabase<SuperposedGameStateC<Realisation>>::getInstance().getOrInsert(option).getId());
    }
    std::vector<SuperposedGameStateId> superposedOptions = getSuperposedOptions<SuperposedGameStateC<Realisation>>(player, width);
    result.insert(result.end(), superposedOptions.begin(), superposedOptions.end());

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
};

#endif // SUPERPOSED_GAME_STATE_C_H
