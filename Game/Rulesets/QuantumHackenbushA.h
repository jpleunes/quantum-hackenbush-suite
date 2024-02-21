#ifndef QUANTUM_HACKENBUSH_A_H
#define QUANTUM_HACKENBUSH_A_H

#include "../QuantumHackenbush.h"

template<typename Realisation, typename Piece>
class QuantumHackenbushA : public QuantumHackenbush<Realisation, Piece> {
public:
    QuantumHackenbushA(const Superposition<Realisation, Piece> superposition);
    Generator<QuantumHackenbush<Realisation, Piece>*> options(Player player) const override;

    ~QuantumHackenbushA() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation, typename Piece>
QuantumHackenbushA<Realisation, Piece>::QuantumHackenbushA(const Superposition<Realisation, Piece> superposition) : QuantumHackenbush<Realisation, Piece>(superposition) {
};

template<typename Realisation, typename Piece>
Generator<QuantumHackenbush<Realisation, Piece>*> QuantumHackenbushA<Realisation, Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition.getPieces(player);
    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushA<Realisation, Piece>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

#endif // QUANTUM_HACKENBUSH_A_H
