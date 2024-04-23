#ifndef QUANTUM_HACKENBUSH_C_H
#define QUANTUM_HACKENBUSH_C_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class QuantumHackenbushC : public QuantumHackenbush<Realisation> {
public:
    QuantumHackenbushC(const Superposition<Realisation> superposition);
    bool operator==(const QuantumHackenbushC<Realisation>& other) const;
    std::vector<GameInstanceId> getMoveOptions(Player player) const override;

    ~QuantumHackenbushC() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbushC<Realisation>::QuantumHackenbushC(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
bool QuantumHackenbushC<Realisation>::operator==(const QuantumHackenbushC<Realisation>& other) const {
    return this->superposition == other.superposition;
}

template<typename Realisation>
std::vector<GameInstanceId> QuantumHackenbushC<Realisation>::getMoveOptions(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);
    // Flavour C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    std::vector<GameInstanceId> result;
    for (typename Realisation::Piece piece : pieces) {
        Superposition<Realisation> option;
        bool allValid = true;
        for (PositionId realisationId : this->superposition.getRealisationIds()) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getPosition(realisationId);
            PositionId newRealisationId = realisation.applyMove(piece);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
            else allValid = false;
        }
        if (allValid && !option.empty()) result.emplace_back(GameInstanceDatabase<QuantumHackenbushC<Realisation>>::getInstance().getGameInstanceId(QuantumHackenbushC<Realisation>(option)));
    }
    std::vector<GameInstanceId> superposedMoveOptions = getSuperposedMoveOptions<QuantumHackenbushC<Realisation>>(player);
    result.insert(result.end(), superposedMoveOptions.begin(), superposedMoveOptions.end());
    return result;
};

namespace std {
    template<typename Realisation>
    struct hash<QuantumHackenbushC<Realisation>> {
        size_t operator()(const QuantumHackenbushC<Realisation>& quantumHackenbush) const {
            return std::hash<Superposition<Realisation>>()(quantumHackenbush.getSuperposition());
        }
    };
}

#endif // QUANTUM_HACKENBUSH_C_H
