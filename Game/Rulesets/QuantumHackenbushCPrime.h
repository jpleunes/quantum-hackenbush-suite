#ifndef QUANTUM_HACKENBUSH_C_PRIME_H
#define QUANTUM_HACKENBUSH_C_PRIME_H

#include "../QuantumHackenbush.h"

template<typename Piece>
class QuantumHackenbushCPrime : public QuantumHackenbush<Piece> {
public:
    QuantumHackenbushCPrime(const Superposition<Piece>* superposition);
    Generator<QuantumHackenbush<Piece>*> options(Player player) const override;

    ~QuantumHackenbushCPrime() override;
};

// This is a templated class, so the implementations need to go here

template<typename Piece>
QuantumHackenbushCPrime<Piece>::QuantumHackenbushCPrime(const Superposition<Piece>* superposition) : QuantumHackenbush<Piece>(superposition) {
};

template<typename Piece>
Generator<QuantumHackenbush<Piece>*> QuantumHackenbushCPrime<Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition->getPieces(player);

    // Ruleset C': unsuperposed moves are allowed if and only if they are valid in all possible realisations
    // where he still has at least one classical move
    for (Piece piece : pieces) {
        Superposition<Piece> *option = new Superposition<Piece>();
        bool allValidWithClassicalMove = true;
        for (size_t i = 0; i < this->superposition->getWidth(); i++) {
            if (this->superposition->getRealisation(i).getPieces(player).empty()) continue;
            
            Position<Piece> *newRealisation = this->superposition->getRealisation(i).applyMove(piece);
            if (newRealisation != nullptr) option->addRealisation(newRealisation);
            else allValidWithClassicalMove = false;
        }
        if (allValidWithClassicalMove && option->getWidth() > 0) co_yield new QuantumHackenbushCPrime<Piece>(option);
        else delete option;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushCPrime<Piece>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

template<typename Piece>
QuantumHackenbushCPrime<Piece>::~QuantumHackenbushCPrime() {
    delete this->superposition;
};

#endif // QUANTUM_HACKENBUSH_C_PRIME_H
