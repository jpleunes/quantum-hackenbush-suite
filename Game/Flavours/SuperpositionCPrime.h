#ifndef SUPERPOSITION_C_PRIME_H
#define SUPERPOSITION_C_PRIME_H

#include "../Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionCPrime : public Superposition<Realisation> {
public:
    SuperpositionCPrime() = default;
    SuperpositionCPrime(PositionId positionId);
    std::vector<SuperpositionId> getMoveOptions(Player player) const override;

    ~SuperpositionCPrime() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionCPrime<Realisation>::SuperpositionCPrime(PositionId positionId) : Superposition<Realisation>(positionId) {
};

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionCPrime<Realisation>::getMoveOptions(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->getPieces(player);
    // Flavour C': unsuperposed moves are allowed if and only if they are valid in all possible realisations
    // where he still has at least one classical move
    std::vector<SuperpositionId> result;
    for (typename Realisation::Piece piece : pieces) {
        SuperpositionCPrime option;
        bool allValidWithClassicalMove = true;
        for (PositionId realisationId : this->realisationIds) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getPosition(realisationId);
            if (realisation.getPieces(player).empty()) continue;
            
            PositionId newRealisationId = realisation.applyMove(piece);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
            else allValidWithClassicalMove = false;
        }
        if (allValidWithClassicalMove && !option.empty()) result.emplace_back(SuperpositionDatabase<SuperpositionCPrime<Realisation>>::getInstance().getSuperpositionId(SuperpositionCPrime<Realisation>(option)));
    }

    std::vector<SuperpositionId> superposedMoveOptions = getSuperposedMoveOptions<SuperpositionCPrime<Realisation>>(player);
    result.insert(result.end(), superposedMoveOptions.begin(), superposedMoveOptions.end());
    return result;
};

namespace std {
    template<typename Realisation>
    struct hash<SuperpositionCPrime<Realisation>> {
        size_t operator()(const SuperpositionCPrime<Realisation>& superposition) const {
            return std::hash<Superposition<Realisation>>()(superposition);
        }
    };
}

#endif // SUPERPOSITION_C_PRIME_H
