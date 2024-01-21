#ifndef QUANTUM_HACKENBUSH_C_H
#define QUANTUM_HACKENBUSH_C_H

#include "../QuantumHackenbush.h"

template<typename Piece>
class QuantumHackenbushC : public QuantumHackenbush<Piece> {
public:
    QuantumHackenbushC(const Superposition<Piece>* superposition);
    Generator<QuantumHackenbush<Piece>*> options(Player player) const override;

    ~QuantumHackenbushC() override;
};

// This is a templated class, so the implementations need to go here

template<typename Piece>
QuantumHackenbushC<Piece>::QuantumHackenbushC(const Superposition<Piece>* superposition) : QuantumHackenbush<Piece>(superposition) {
};

template<typename Piece>
Generator<QuantumHackenbush<Piece>*> QuantumHackenbushC<Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition->getPieces(player);

    // Ruleset C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    for (Piece piece : pieces) {
        Superposition<Piece> *option = new Superposition<Piece>();
        bool allValid = true;
        for (size_t i = 0; i < this->superposition->getWidth(); i++) {
            Position<Piece> *newRealisation = this->superposition->getRealisation(i).applyMove(piece);
            if (newRealisation != nullptr) option->addRealisation(newRealisation);
            else allValid = false;
        }
        if (allValid && option->getWidth() > 0) co_yield new QuantumHackenbushC<Piece>(option);
        else delete option;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushC<Piece>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

template<typename Piece>
QuantumHackenbushC<Piece>::~QuantumHackenbushC() {
    delete this->superposition;
};

#endif // QUANTUM_HACKENBUSH_C_H
