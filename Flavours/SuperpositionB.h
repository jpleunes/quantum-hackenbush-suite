#ifndef SUPERPOSITION_B_H
#define SUPERPOSITION_B_H

#include "Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionB : public Superposition<Realisation> {
public:
    SuperpositionB(HackenbushId positionId, SuperpositionId id);
    SuperpositionB(std::set<HackenbushId> realisations, SuperpositionId id);
    std::vector<SuperpositionId> getOptions(Player player) const override;

    ~SuperpositionB() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionB<Realisation>::SuperpositionB(HackenbushId positionId, SuperpositionId id) : Superposition<Realisation>(positionId, id) {
};

template<typename Realisation>
SuperpositionB<Realisation>::SuperpositionB(std::set<HackenbushId> realisations, SuperpositionId id) : Superposition<Realisation>(realisations, id) {
}

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionB<Realisation>::getOptions(Player player) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();

    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour B: if a player has only one possible move within all realisations together, 
    // they can play it as an unsuperposed move
    std::vector<SuperpositionId> result;
    if (pieces.size() == 1) {
        std::set<HackenbushId> option;
        for (HackenbushId realisationId : this->getRealisations()) {
            Hackenbush<Realisation> realisation = HackenbushDatabase<Realisation>::getInstance().getPosition(realisationId);
            std::optional<HackenbushId> newRealisationId = realisation.applyMove(pieces[0]);
            if (newRealisationId.has_value()) option.insert(newRealisationId.value());
        }
        result.emplace_back(SuperpositionDatabase<SuperpositionB<Realisation>>::getInstance().getOrInsert(option).getId());

        if (player == Player::LEFT) this->cache.leftOptions = result;
        else if (player == Player::RIGHT) this->cache.rightOptions = result;

        return result;
    }
    result = getSuperposedOptions<SuperpositionB<Realisation>>(player);

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
};

#endif // SUPERPOSITION_B_H
