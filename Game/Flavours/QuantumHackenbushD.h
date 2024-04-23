#ifndef QUANTUM_HACKENBUSH_D_H
#define QUANTUM_HACKENBUSH_D_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class QuantumHackenbushD : public QuantumHackenbush<Realisation> {
public:
    QuantumHackenbushD(const Superposition<Realisation> superposition);
    bool operator==(const QuantumHackenbushD<Realisation>& other) const;
    std::vector<GameInstanceId> getMoveOptions(Player player) const override;

    ~QuantumHackenbushD() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbushD<Realisation>::QuantumHackenbushD(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
bool QuantumHackenbushD<Realisation>::operator==(const QuantumHackenbushD<Realisation>& other) const {
    return this->superposition == other.superposition;
}

template<typename Realisation>
std::vector<GameInstanceId> QuantumHackenbushD<Realisation>::getMoveOptions(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);
    // Flavour D: unsuperposed moves are always allowed
    std::vector<GameInstanceId> result;
    for (typename Realisation::Piece piece : pieces) {
        Superposition<Realisation> option;
        for (PositionId realisationId : this->superposition.getRealisationIds()) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getPosition(realisationId);
            PositionId newRealisationId = realisation.applyMove(piece);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
        }
        if (!option.empty()) result.emplace_back(GameInstanceDatabase<QuantumHackenbushD<Realisation>>::getInstance().getGameInstanceId(QuantumHackenbushD<Realisation>(option)));
    }
    std::vector<GameInstanceId> superposedMoveOptions = getSuperposedMoveOptions<QuantumHackenbushD<Realisation>>(player);
    result.insert(result.end(), superposedMoveOptions.begin(), superposedMoveOptions.end());
    return result;
};

namespace std {
    template<typename Realisation>
    struct hash<QuantumHackenbushD<Realisation>> {
        size_t operator()(const QuantumHackenbushD<Realisation>& quantumHackenbush) const {
            return std::hash<Superposition<Realisation>>()(quantumHackenbush.getSuperposition());
        }
    };
}

#endif // QUANTUM_HACKENBUSH_D_H
