#ifndef SUPERPOSITION_D_H
#define SUPERPOSITION_D_H

#include "Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionD : public Superposition<Realisation> {
public:
    SuperpositionD(HackenbushId positionId, SuperpositionId id);
    SuperpositionD(std::set<HackenbushId> realisations, SuperpositionId id);
    std::vector<SuperpositionId> getOptions(Player player) const override;

    ~SuperpositionD() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionD<Realisation>::SuperpositionD(HackenbushId positionId, SuperpositionId id) : Superposition<Realisation>(positionId, id) {
};

template<typename Realisation>
SuperpositionD<Realisation>::SuperpositionD(std::set<HackenbushId> realisations, SuperpositionId id) : Superposition<Realisation>(realisations, id) {
}

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionD<Realisation>::getOptions(Player player) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();

    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour D: unsuperposed moves are always allowed
    std::vector<SuperpositionId> result;
    for (typename Realisation::Piece piece : pieces) {
        std::set<HackenbushId> option;
        for (HackenbushId realisationId : this->getRealisations()) {
            Hackenbush<Realisation> realisation = HackenbushDatabase<Realisation>::getInstance().getPosition(realisationId);
            std::optional<HackenbushId> newRealisationId = realisation.applyMove(piece);
            if (newRealisationId.has_value()) option.insert(newRealisationId.value());
        }
        if (!option.empty()) result.emplace_back(SuperpositionDatabase<SuperpositionD<Realisation>>::getInstance().getOrInsert(option).getId());
    }
    std::vector<SuperpositionId> superposedOptions = getSuperposedOptions<SuperpositionD<Realisation>>(player);
    result.insert(result.end(), superposedOptions.begin(), superposedOptions.end());

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
};

#endif // SUPERPOSITION_D_H
