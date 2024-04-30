#ifndef SUPERPOSITION_C_H
#define SUPERPOSITION_C_H

#include "Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionC : public Superposition<Realisation> {
public:
    SuperpositionC(HackenbushId positionId, SuperpositionId);
    SuperpositionC(std::set<HackenbushId> realisations, SuperpositionId id);
    std::vector<SuperpositionId> getOptions(Player player) const override;

    ~SuperpositionC() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionC<Realisation>::SuperpositionC(HackenbushId positionId, SuperpositionId id) : Superposition<Realisation>(positionId) {
};

template<typename Realisation>
SuperpositionC<Realisation>::SuperpositionC(std::set<HackenbushId> realisations, SuperpositionId id) : Superposition<Realisation>(realisations, id) {
}

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionC<Realisation>::getOptions(Player player) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();

    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    std::vector<SuperpositionId> result;
    for (typename Realisation::Piece piece : pieces) {
        std::set<HackenbushId> option;
        bool allValid = true;
        for (HackenbushId realisationId : this->getRealisations()) {
            Hackenbush<Realisation> realisation = HackenbushDatabase<Realisation>::getInstance().getPosition(realisationId);
            std::optional<HackenbushId> newRealisationId = realisation.applyMove(piece);
            if (newRealisationId.has_value()) option.insert(newRealisationId.value());
            else allValid = false;
        }
        if (allValid && !option.empty()) result.emplace_back(SuperpositionDatabase<SuperpositionC<Realisation>>::getInstance().getOrInsert(option).getId());
    }
    std::vector<SuperpositionId> superposedOptions = getSuperposedOptions<SuperpositionC<Realisation>>(player);
    result.insert(result.end(), superposedOptions.begin(), superposedOptions.end());

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
};

#endif // SUPERPOSITION_C_H
