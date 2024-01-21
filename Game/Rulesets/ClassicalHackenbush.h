#ifndef CLASSICAL_HACKENBUSH_H
#define CLASSICAL_HACKENBUSH_H

#include "../QuantumHackenbush.h"

template<typename Piece>
class ClassicalHackenbush : public QuantumHackenbush<Piece> {
public:
    ClassicalHackenbush(const Superposition<Piece>* superposition);
    Generator<QuantumHackenbush<Piece>*> options(Player player) const override;

    ~ClassicalHackenbush() override;
};

// This is a templated class, so the implementations need to go here

template<typename Piece>
ClassicalHackenbush<Piece>::ClassicalHackenbush(const Superposition<Piece>* superposition) : QuantumHackenbush<Piece>(superposition) {
};

template<typename Piece>
Generator<QuantumHackenbush<Piece>*> ClassicalHackenbush<Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition->getPieces(player);
    for (size_t i = 0; i < pieces.size(); i++) {
        Position<Piece> *newPosition = this->superposition->getRealisation(0).applyMove(pieces[i]);
        co_yield new ClassicalHackenbush<Piece>(new Superposition<Piece>(newPosition));
    }
};

template<typename Piece>
ClassicalHackenbush<Piece>::~ClassicalHackenbush() {
    delete this->superposition;
};

#endif // CLASSICAL_HACKENBUSH_H
