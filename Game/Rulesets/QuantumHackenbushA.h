#ifndef QUANTUM_HACKENBUSH_A_H
#define QUANTUM_HACKENBUSH_A_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class QuantumHackenbushA : public QuantumHackenbush<Realisation> {
public:
    QuantumHackenbushA(const Superposition<Realisation> superposition);
    Generator<QuantumHackenbush<Realisation>*> options(Player player) const override;

    ~QuantumHackenbushA() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbushA<Realisation>::QuantumHackenbushA(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
Generator<QuantumHackenbush<Realisation>*> QuantumHackenbushA<Realisation>::options(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);
    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushA<Realisation>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

#endif // QUANTUM_HACKENBUSH_A_H
