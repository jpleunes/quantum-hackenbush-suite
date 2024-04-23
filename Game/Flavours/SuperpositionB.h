#ifndef SUPERPOSITION_B_H
#define SUPERPOSITION_B_H

#include "../Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionB : public Superposition<Realisation> {
public:
    SuperpositionB() = default;
    SuperpositionB(PositionId positionId);
    std::vector<SuperpositionId> getMoveOptions(Player player) const override;

    ~SuperpositionB() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionB<Realisation>::SuperpositionB(PositionId positionId) : Superposition<Realisation>(positionId) {
};

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionB<Realisation>::getMoveOptions(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour B: if a player has only one possible move within all realisations together, 
    // he can play it as an unsuperposed move
    std::vector<SuperpositionId> result;
    if (pieces.size() == 1) {
        SuperpositionB option;
        for (PositionId realisationId : this->realisationIds) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getPosition(realisationId);
            PositionId newRealisationId = realisation.applyMove(pieces[0]);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
        }
        result.emplace_back(SuperpositionDatabase<SuperpositionB<Realisation>>::getInstance().getSuperpositionId(SuperpositionB<Realisation>(option)));
        return result;
    }
    return getSuperposedMoveOptions<SuperpositionB<Realisation>>(player);
};

namespace std {
    template<typename Realisation>
    struct hash<SuperpositionB<Realisation>> {
        size_t operator()(const SuperpositionB<Realisation>& superposition) const {
            return std::hash<Superposition<Realisation>>()(superposition);
        }
    };
}

#endif // SUPERPOSITION_B_H
