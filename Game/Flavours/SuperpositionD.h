#ifndef SUPERPOSITION_D_H
#define SUPERPOSITION_D_H

#include "../Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionD : public Superposition<Realisation> {
public:
    SuperpositionD() = default;
    SuperpositionD(PositionId positionId);
    std::vector<SuperpositionId> getMoveOptions(Player player) const override;

    ~SuperpositionD() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionD<Realisation>::SuperpositionD(PositionId positionId) : Superposition<Realisation>(positionId) {
};

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionD<Realisation>::getMoveOptions(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour D: unsuperposed moves are always allowed
    std::vector<SuperpositionId> result;
    for (typename Realisation::Piece piece : pieces) {
        SuperpositionD option;
        for (PositionId realisationId : this->realisationIds) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getPosition(realisationId);
            PositionId newRealisationId = realisation.applyMove(piece);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
        }
        if (!option.empty()) result.emplace_back(SuperpositionDatabase<SuperpositionD<Realisation>>::getInstance().getSuperpositionId(SuperpositionD<Realisation>(option)));
    }
    std::vector<SuperpositionId> superposedMoveOptions = getSuperposedMoveOptions<SuperpositionD<Realisation>>(player);
    result.insert(result.end(), superposedMoveOptions.begin(), superposedMoveOptions.end());
    return result;
};

namespace std {
    template<typename Realisation>
    struct hash<SuperpositionD<Realisation>> {
        size_t operator()(const SuperpositionD<Realisation>& superposition) const {
            return std::hash<Superposition<Realisation>>()(superposition);
        }
    };
}

#endif // SUPERPOSITION_D_H
