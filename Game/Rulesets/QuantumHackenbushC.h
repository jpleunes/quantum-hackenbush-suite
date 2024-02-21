#ifndef QUANTUM_HACKENBUSH_C_H
#define QUANTUM_HACKENBUSH_C_H

#include "../QuantumHackenbush.h"

template<typename Realisation, typename Piece>
class QuantumHackenbushC : public QuantumHackenbush<Realisation, Piece> {
public:
    QuantumHackenbushC(const Superposition<Realisation, Piece> superposition);
    Generator<QuantumHackenbush<Realisation, Piece>*> options(Player player) const override;

    ~QuantumHackenbushC() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation, typename Piece>
QuantumHackenbushC<Realisation, Piece>::QuantumHackenbushC(const Superposition<Realisation, Piece> superposition) : QuantumHackenbush<Realisation, Piece>(superposition) {
};

template<typename Realisation, typename Piece>
Generator<QuantumHackenbush<Realisation, Piece>*> QuantumHackenbushC<Realisation, Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition.getPieces(player);

    // Ruleset C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    for (Piece piece : pieces) {
        Superposition<Realisation, Piece> option;
        bool allValid = true;
        for (const Realisation* realisation : this->superposition.getRealisations()) {
            auto* newRealisationPtr = realisation->applyMove(piece);
            if (newRealisationPtr != nullptr) {
                Realisation newRealisation = std::move(*static_cast<Realisation*>(newRealisationPtr));
                delete newRealisationPtr;
                option.addRealisation(newRealisation);
            }
            else allValid = false;
        }
        if (allValid && !option.empty()) co_yield new QuantumHackenbushC<Realisation, Piece>(option);
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushC<Realisation, Piece>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

#endif // QUANTUM_HACKENBUSH_C_H
