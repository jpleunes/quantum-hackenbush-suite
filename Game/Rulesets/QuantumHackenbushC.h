#ifndef QUANTUM_HACKENBUSH_C_H
#define QUANTUM_HACKENBUSH_C_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class QuantumHackenbushC : public QuantumHackenbush<Realisation> {
public:
    QuantumHackenbushC(const Superposition<Realisation> superposition);
    Generator<QuantumHackenbush<Realisation>*> options(Player player) const override;

    ~QuantumHackenbushC() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbushC<Realisation>::QuantumHackenbushC(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
Generator<QuantumHackenbush<Realisation>*> QuantumHackenbushC<Realisation>::options(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);

    // Ruleset C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    for (typename Realisation::Piece piece : pieces) {
        Superposition<Realisation> option;
        bool allValid = true;
        for (PositionId realisationId : this->superposition.getRealisationIds()) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getGame(realisationId);
            PositionId newRealisationId = realisation.applyMove(piece);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
            else allValid = false;
        }
        if (allValid && !option.empty()) co_yield new QuantumHackenbushC<Realisation>(option);
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushC<Realisation>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

#endif // QUANTUM_HACKENBUSH_C_H
