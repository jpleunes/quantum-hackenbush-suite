#ifndef SUPERPOSITION_C_H
#define SUPERPOSITION_C_H

#include "../Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionC : public Superposition<Realisation> {
public:
    SuperpositionC() = default;
    SuperpositionC(PositionId positionId);
    std::vector<SuperpositionId> getMoveOptions(Player player) const override;

    ~SuperpositionC() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionC<Realisation>::SuperpositionC(PositionId positionId) : Superposition<Realisation>(positionId) {
};

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionC<Realisation>::getMoveOptions(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    std::vector<SuperpositionId> result;
    for (typename Realisation::Piece piece : pieces) {
        SuperpositionC option;
        bool allValid = true;
        for (PositionId realisationId : this->realisationIds) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getPosition(realisationId);
            PositionId newRealisationId = realisation.applyMove(piece);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
            else allValid = false;
        }
        if (allValid && !option.empty()) result.emplace_back(SuperpositionDatabase<SuperpositionC<Realisation>>::getInstance().getSuperpositionId(SuperpositionC<Realisation>(option)));
    }
    std::vector<SuperpositionId> superposedMoveOptions = getSuperposedMoveOptions<SuperpositionC<Realisation>>(player);
    result.insert(result.end(), superposedMoveOptions.begin(), superposedMoveOptions.end());
    return result;
};

namespace std {
    template<typename Realisation>
    struct hash<SuperpositionC<Realisation>> {
        size_t operator()(const SuperpositionC<Realisation>& superposition) const {
            return std::hash<Superposition<Realisation>>()(superposition);
        }
    };
}

#endif // SUPERPOSITION_C_H
