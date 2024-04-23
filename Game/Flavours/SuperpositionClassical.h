#ifndef SUPERPOSITION_CLASSICAL_H
#define SUPERPOSITION_CLASSICAL_H

#include "../Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionClassical : public Superposition<Realisation> {
public:
    SuperpositionClassical() = default;
    SuperpositionClassical(PositionId positionId);
    std::vector<SuperpositionId> getMoveOptions(Player player) const override;

    ~SuperpositionClassical() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionClassical<Realisation>::SuperpositionClassical(PositionId positionId) : Superposition<Realisation>(positionId) {
};

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionClassical<Realisation>::getMoveOptions(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    std::vector<SuperpositionId> result;
    for (size_t i = 0; i < pieces.size(); i++) {
        Realisation& position = PositionDatabase<Realisation>::getInstance().getPosition(*this->realisationIds.begin());
        PositionId newPositionId = position.applyMove(pieces[i]);
        SuperpositionId superpositionId = SuperpositionDatabase<SuperpositionClassical<Realisation>>::getInstance().getSuperpositionId(SuperpositionClassical<Realisation>(newPositionId));
        result.emplace_back(superpositionId);
    }
    return result;
};

namespace std {
    template<typename Realisation>
    struct hash<SuperpositionClassical<Realisation>> {
        size_t operator()(const SuperpositionClassical<Realisation>& superpositionClassical) const {
            return std::hash<Superposition<Realisation>>()(superpositionClassical);
        }
    };
}

#endif // SUPERPOSITION_CLASSICAL_H
