#ifndef SUPERPOSITION_A_H
#define SUPERPOSITION_A_H

#include "Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionA : public Superposition<Realisation> {
public:
    SuperpositionA(HackenbushId positionId, SuperpositionId id);
    SuperpositionA(std::set<HackenbushId> realisations, SuperpositionId id);
    std::vector<SuperpositionId> getOptions(Player player) const override;

    ~SuperpositionA() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionA<Realisation>::SuperpositionA(HackenbushId positionId, SuperpositionId id) : Superposition<Realisation>(positionId, id) {
};

template<typename Realisation>
SuperpositionA<Realisation>::SuperpositionA(std::set<HackenbushId> realisations, SuperpositionId id) : Superposition<Realisation>(realisations, id) {
}

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionA<Realisation>::getOptions(Player player) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();
    
    std::vector<SuperpositionId> result = getSuperposedOptions<SuperpositionA<Realisation>>(player);

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
};

#endif // SUPERPOSITION_A_H
