#ifndef QUANTUM_HACKENBUSH_C_PRIME_H
#define QUANTUM_HACKENBUSH_C_PRIME_H

#include "../QuantumHackenbush.h"

template<typename Realisation, typename Piece>
class QuantumHackenbushCPrime : public QuantumHackenbush<Realisation, Piece> {
public:
    QuantumHackenbushCPrime(const Superposition<Realisation, Piece> superposition);
    Generator<QuantumHackenbush<Realisation, Piece>*> options(Player player) const override;

    ~QuantumHackenbushCPrime() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation, typename Piece>
QuantumHackenbushCPrime<Realisation, Piece>::QuantumHackenbushCPrime(const Superposition<Realisation, Piece> superposition) : QuantumHackenbush<Realisation, Piece>(superposition) {
};

template<typename Realisation, typename Piece>
Generator<QuantumHackenbush<Realisation, Piece>*> QuantumHackenbushCPrime<Realisation, Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition.getPieces(player);

    // Ruleset C': unsuperposed moves are allowed if and only if they are valid in all possible realisations
    // where he still has at least one classical move
    for (Piece piece : pieces) {
        Superposition<Realisation, Piece> option;
        bool allValidWithClassicalMove = true;
        for (const Realisation* realisation : this->superposition.getRealisations()) {
            if (realisation->getPieces(player).empty()) continue;
            
            auto* newRealisationPtr = realisation->applyMove(piece);
            if (newRealisationPtr != nullptr) {
                Realisation newRealisation = std::move(*static_cast<Realisation*>(newRealisationPtr));
                delete newRealisationPtr;
                option.addRealisation(newRealisation);
            }
            else allValidWithClassicalMove = false;
        }
        if (allValidWithClassicalMove && !option.empty()) co_yield new QuantumHackenbushCPrime<Realisation, Piece>(option);
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushCPrime<Realisation, Piece>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

#endif // QUANTUM_HACKENBUSH_C_PRIME_H
