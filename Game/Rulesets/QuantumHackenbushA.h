#ifndef QUANTUM_HACKENBUSH_A_H
#define QUANTUM_HACKENBUSH_A_H

#include "../QuantumHackenbush.h"

template<typename Piece>
class QuantumHackenbushA : public QuantumHackenbush<Piece> {
public:
    QuantumHackenbushA(const Superposition<Piece>* superposition);
    Generator<QuantumHackenbush<Piece>*> options(Player player) const override;

    ~QuantumHackenbushA() override;
};

// This is a templated class, so the implementations need to go here

template<typename Piece>
QuantumHackenbushA<Piece>::QuantumHackenbushA(const Superposition<Piece>* superposition) : QuantumHackenbush<Piece>(superposition) {
};

template<typename Piece>
Generator<QuantumHackenbush<Piece>*> QuantumHackenbushA<Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition->getPieces(player);
    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushA<Piece>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

template<typename Piece>
QuantumHackenbushA<Piece>::~QuantumHackenbushA() {
    delete this->superposition;
};

#endif // QUANTUM_HACKENBUSH_A_H
