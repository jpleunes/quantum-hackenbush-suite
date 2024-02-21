#ifndef QUANTUM_HACKENBUSH_B_H
#define QUANTUM_HACKENBUSH_B_H

#include "../QuantumHackenbush.h"

template<typename Realisation, typename Piece>
class QuantumHackenbushB : public QuantumHackenbush<Realisation, Piece> {
public:
    QuantumHackenbushB(const Superposition<Realisation, Piece> superposition);
    Generator<QuantumHackenbush<Realisation, Piece>*> options(Player player) const override;

    ~QuantumHackenbushB() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation, typename Piece>
QuantumHackenbushB<Realisation, Piece>::QuantumHackenbushB(const Superposition<Realisation, Piece> superposition) : QuantumHackenbush<Realisation, Piece>(superposition) {
};

template<typename Realisation, typename Piece>
Generator<QuantumHackenbush<Realisation, Piece>*> QuantumHackenbushB<Realisation, Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition.getPieces(player);

    // Ruleset B: if a player has only one possible move within all realisations together, 
    // he can play it as an unsuperposed move
    if (pieces.size() == 1) {
        Superposition<Realisation, Piece> option;
        for (const Realisation* realisation : this->superposition.getRealisations()) {
            auto* newRealisationPtr = realisation->applyMove(pieces[0]);
            if (newRealisationPtr != nullptr) {
                Realisation newRealisation = std::move(*static_cast<Realisation*>(newRealisationPtr));
                delete newRealisationPtr;
                option.addRealisation(newRealisation);
            }
        }
        co_yield new QuantumHackenbushB<Realisation, Piece>(option);
        co_return;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushB<Realisation, Piece>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

#endif // QUANTUM_HACKENBUSH_B_H
