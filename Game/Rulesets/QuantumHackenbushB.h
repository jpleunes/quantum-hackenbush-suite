#ifndef QUANTUM_HACKENBUSH_B_H
#define QUANTUM_HACKENBUSH_B_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class QuantumHackenbushB : public QuantumHackenbush<Realisation> {
public:
    QuantumHackenbushB(const Superposition<Realisation> superposition);
    bool operator==(const QuantumHackenbushB<Realisation>& other) const;
    std::vector<GameInstanceId> getMoveOptions(Player player) const override;

    ~QuantumHackenbushB() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbushB<Realisation>::QuantumHackenbushB(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
bool QuantumHackenbushB<Realisation>::operator==(const QuantumHackenbushB<Realisation>& other) const {
    return this->superposition == other.superposition;
}

template<typename Realisation>
std::vector<GameInstanceId> QuantumHackenbushB<Realisation>::getMoveOptions(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);
    // Ruleset B: if a player has only one possible move within all realisations together, 
    // he can play it as an unsuperposed move
    std::vector<GameInstanceId> result;
    if (pieces.size() == 1) {
        Superposition<Realisation> option;
        for (PositionId realisationId : this->superposition.getRealisationIds()) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getPosition(realisationId);
            PositionId newRealisationId = realisation.applyMove(pieces[0]);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
        }
        result.emplace_back(GameInstanceDatabase<QuantumHackenbushB<Realisation>>::getInstance().getGameInstanceId(QuantumHackenbushB<Realisation>(option)));
        return result;
    }
    return getSuperposedMoveOptions<QuantumHackenbushB<Realisation>>(player);
};

namespace std {
    template<typename Realisation>
    struct hash<QuantumHackenbushB<Realisation>> {
        size_t operator()(const QuantumHackenbushB<Realisation>& quantumHackenbush) const {
            return std::hash<Superposition<Realisation>>()(quantumHackenbush.getSuperposition());
        }
    };
}

#endif // QUANTUM_HACKENBUSH_B_H
