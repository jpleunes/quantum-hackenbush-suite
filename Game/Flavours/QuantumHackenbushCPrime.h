#ifndef QUANTUM_HACKENBUSH_C_PRIME_H
#define QUANTUM_HACKENBUSH_C_PRIME_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class QuantumHackenbushCPrime : public QuantumHackenbush<Realisation> {
public:
    QuantumHackenbushCPrime(const Superposition<Realisation> superposition);
    bool operator==(const QuantumHackenbushCPrime<Realisation>& other) const;
    std::vector<GameInstanceId> getMoveOptions(Player player) const override;

    ~QuantumHackenbushCPrime() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbushCPrime<Realisation>::QuantumHackenbushCPrime(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
bool QuantumHackenbushCPrime<Realisation>::operator==(const QuantumHackenbushCPrime<Realisation>& other) const {
    return this->superposition == other.superposition;
}

template<typename Realisation>
std::vector<GameInstanceId> QuantumHackenbushCPrime<Realisation>::getMoveOptions(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);
    // Flavour C': unsuperposed moves are allowed if and only if they are valid in all possible realisations
    // where he still has at least one classical move
    std::vector<GameInstanceId> result;
    for (typename Realisation::Piece piece : pieces) {
        Superposition<Realisation> option;
        bool allValidWithClassicalMove = true;
        for (PositionId realisationId : this->superposition.getRealisationIds()) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getPosition(realisationId);
            if (realisation.getPieces(player).empty()) continue;
            
            PositionId newRealisationId = realisation.applyMove(piece);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
            else allValidWithClassicalMove = false;
        }
        if (allValidWithClassicalMove && !option.empty()) result.emplace_back(GameInstanceDatabase<QuantumHackenbushCPrime<Realisation>>::getInstance().getGameInstanceId(QuantumHackenbushCPrime<Realisation>(option)));
    }

    std::vector<GameInstanceId> superposedMoveOptions = getSuperposedMoveOptions<QuantumHackenbushCPrime<Realisation>>(player);
    result.insert(result.end(), superposedMoveOptions.begin(), superposedMoveOptions.end());
    return result;
};

namespace std {
    template<typename Realisation>
    struct hash<QuantumHackenbushCPrime<Realisation>> {
        size_t operator()(const QuantumHackenbushCPrime<Realisation>& quantumHackenbush) const {
            return std::hash<Superposition<Realisation>>()(quantumHackenbush.getSuperposition());
        }
    };
}

#endif // QUANTUM_HACKENBUSH_C_PRIME_H
