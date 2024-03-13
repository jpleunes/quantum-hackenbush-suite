#ifndef QUANTUM_HACKENBUSH_B_H
#define QUANTUM_HACKENBUSH_B_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class QuantumHackenbushB : public QuantumHackenbush<Realisation> {
public:
    QuantumHackenbushB(const Superposition<Realisation> superposition);
    Generator<QuantumHackenbush<Realisation>*> options(Player player) const override;

    ~QuantumHackenbushB() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbushB<Realisation>::QuantumHackenbushB(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
Generator<QuantumHackenbush<Realisation>*> QuantumHackenbushB<Realisation>::options(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);

    // Ruleset B: if a player has only one possible move within all realisations together, 
    // he can play it as an unsuperposed move
    if (pieces.size() == 1) {
        Superposition<Realisation> option;
        for (PositionId realisationId : this->superposition.getRealisationIds()) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getGame(realisationId);
            PositionId newRealisationId = realisation.applyMove(pieces[0]);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
        }
        co_yield new QuantumHackenbushB<Realisation>(option);
        co_return;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushB<Realisation>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

#endif // QUANTUM_HACKENBUSH_B_H
