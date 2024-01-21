#ifndef QUANTUM_HACKENBUSH_B_H
#define QUANTUM_HACKENBUSH_B_H

#include "../QuantumHackenbush.h"

template<typename Piece>
class QuantumHackenbushB : public QuantumHackenbush<Piece> {
public:
    QuantumHackenbushB(const Superposition<Piece>* superposition);
    Generator<QuantumHackenbush<Piece>*> options(Player player) const override;

    ~QuantumHackenbushB() override;
};

// This is a templated class, so the implementations need to go here

template<typename Piece>
QuantumHackenbushB<Piece>::QuantumHackenbushB(const Superposition<Piece>* superposition) : QuantumHackenbush<Piece>(superposition) {
};

template<typename Piece>
Generator<QuantumHackenbush<Piece>*> QuantumHackenbushB<Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition->getPieces(player);

    // Ruleset B: if a player has only one possible move within all realisations together, 
    // he can play it as an unsuperposed move
    if (pieces.size() == 1) {
        Superposition<Piece> *option = new Superposition<Piece>();
        for (size_t i = 0; i < this->superposition->getWidth(); i++) {
            Position<Piece> *newRealisation = this->superposition->getRealisation(i).applyMove(pieces[0]);
            if (newRealisation != nullptr) option->addRealisation(newRealisation);
        }
        co_yield new QuantumHackenbushB<Piece>(option);
        co_return;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushB<Piece>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

template<typename Piece>
QuantumHackenbushB<Piece>::~QuantumHackenbushB() {
    delete this->superposition;
};

#endif // QUANTUM_HACKENBUSH_B_H
