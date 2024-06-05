#ifndef SUPERPOSED_GAME_STATE_C_PRIME_H
#define SUPERPOSED_GAME_STATE_C_PRIME_H

#include "SuperposedGameState/SuperposedGameState.h"

template<typename Realisation>
class SuperposedGameStateCPrime : public SuperposedGameState<Realisation> {
public:
    SuperposedGameStateCPrime(GameStateId classicalGameState, SuperposedGameStateId id);
    SuperposedGameStateCPrime(std::set<GameStateId> realisations, SuperposedGameStateId id);
    std::vector<SuperposedGameStateId> getOptions(Player player, size_t width) const override;

    ~SuperposedGameStateCPrime() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperposedGameStateCPrime<Realisation>::SuperposedGameStateCPrime(GameStateId classicalGameState, SuperposedGameStateId id) : SuperposedGameState<Realisation>(classicalGameState, id) {
};

template<typename Realisation>
SuperposedGameStateCPrime<Realisation>::SuperposedGameStateCPrime(std::set<GameStateId> realisations, SuperposedGameStateId id) : SuperposedGameState<Realisation>(realisations, id) {
}

template<typename Realisation>
std::vector<SuperposedGameStateId> SuperposedGameStateCPrime<Realisation>::getOptions(Player player, size_t width) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();

    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour C': unsuperposed moves are allowed if and only if they are valid in all possible realisations
    // where they still have at least one classical move
    std::vector<SuperposedGameStateId> result;
    for (typename Realisation::Piece piece : pieces) {
        std::set<GameStateId> option;
        bool allValidWithClassicalMove = true;
        for (GameStateId realisationId : this->getRealisations()) {
            GameState<Realisation> realisation = GameStateDatabase<Realisation>::getInstance().getGameState(realisationId);
            if (realisation.getPieces(player).empty()) continue;
            
            std::optional<GameStateId> newRealisationId = realisation.applyMove(piece);
            if (newRealisationId.has_value()) option.insert(newRealisationId.value());
            else allValidWithClassicalMove = false;
        }
        if (allValidWithClassicalMove && !option.empty()) result.emplace_back(SuperposedGameStateDatabase<SuperposedGameStateCPrime<Realisation>>::getInstance().getOrInsert(option).getId());
    }

    std::vector<SuperposedGameStateId> superposedOptions = getSuperposedOptions<SuperposedGameStateCPrime<Realisation>>(player, width);
    result.insert(result.end(), superposedOptions.begin(), superposedOptions.end());

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
};

#endif // SUPERPOSED_GAME_STATE_C_PRIME_H
