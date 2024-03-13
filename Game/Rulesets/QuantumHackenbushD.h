#ifndef QUANTUM_HACKENBUSH_D_H
#define QUANTUM_HACKENBUSH_D_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class QuantumHackenbushD : public QuantumHackenbush<Realisation> {
public:
    QuantumHackenbushD(const Superposition<Realisation> superposition);
    Generator<QuantumHackenbush<Realisation>*> options(Player player) const override;

    ~QuantumHackenbushD() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbushD<Realisation>::QuantumHackenbushD(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
Generator<QuantumHackenbush<Realisation>*> QuantumHackenbushD<Realisation>::options(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);

    // Ruleset D: unsuperposed moves are always allowed
    for (typename Realisation::Piece piece : pieces) {
        Superposition<Realisation> option;
        for (PositionId realisationId : this->superposition.getRealisationIds()) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getGame(realisationId);
            PositionId newRealisationId = realisation.applyMove(piece);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
        }
        if (!option.empty()) co_yield new QuantumHackenbushD(option);
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushD>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

#endif // QUANTUM_HACKENBUSH_D_H
