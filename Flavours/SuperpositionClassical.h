#ifndef SUPERPOSITION_CLASSICAL_H
#define SUPERPOSITION_CLASSICAL_H

#include "Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionClassical : public Superposition<Realisation> {
public:
    SuperpositionClassical(HackenbushId positionId, SuperpositionId id);
    SuperpositionClassical(std::set<HackenbushId> realisations, SuperpositionId id);
    std::vector<SuperpositionId> getOptions(Player player) const override;

    ~SuperpositionClassical() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionClassical<Realisation>::SuperpositionClassical(HackenbushId positionId, SuperpositionId id) : Superposition<Realisation>(positionId, id) {
};

template<typename Realisation>
SuperpositionClassical<Realisation>::SuperpositionClassical(std::set<HackenbushId> realisations, SuperpositionId id) : Superposition<Realisation>(realisations, id) {
    if (realisations.size() > 1) throw(std::domain_error("Superposition of classical flavour should not contain more than one realisation."));
}

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionClassical<Realisation>::getOptions(Player player) const {
    if (player == Player::LEFT && this->cache.leftOptions.has_value()) return this->cache.leftOptions.value();
    else if (player == Player::RIGHT && this->cache.rightOptions.has_value()) return this->cache.rightOptions.value();

    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    std::vector<SuperpositionId> result;
    for (size_t i = 0; i < pieces.size(); i++) {
        Hackenbush<Realisation>& position = HackenbushDatabase<Realisation>::getInstance().getPosition(*this->getRealisations().begin());
        HackenbushId newPositionId = position.applyMove(pieces[i]).value();
        SuperpositionId superpositionId = SuperpositionDatabase<SuperpositionClassical<Realisation>>::getInstance().getOrInsert(newPositionId).getId();
        result.emplace_back(superpositionId);
    }

    if (player == Player::LEFT) this->cache.leftOptions = result;
    else if (player == Player::RIGHT) this->cache.rightOptions = result;

    return result;
};

#endif // SUPERPOSITION_CLASSICAL_H
