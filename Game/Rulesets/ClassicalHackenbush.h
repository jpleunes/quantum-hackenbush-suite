#ifndef CLASSICAL_HACKENBUSH_H
#define CLASSICAL_HACKENBUSH_H

#include "../QuantumHackenbush.h"

template<typename Realisation, typename Piece>
class ClassicalHackenbush : public QuantumHackenbush<Realisation, Piece> {
public:
    ClassicalHackenbush(const Superposition<Realisation, Piece> superposition);
    Generator<QuantumHackenbush<Realisation, Piece>*> options(Player player) const override;

    ~ClassicalHackenbush() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation, typename Piece>
ClassicalHackenbush<Realisation, Piece>::ClassicalHackenbush(const Superposition<Realisation, Piece> superposition) : QuantumHackenbush<Realisation, Piece>(superposition) {
};

template<typename Realisation, typename Piece>
Generator<QuantumHackenbush<Realisation, Piece>*> ClassicalHackenbush<Realisation, Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition.getPieces(player);
    for (size_t i = 0; i < pieces.size(); i++) {
        auto* newPositionPtr = (*this->superposition.getRealisations().begin())->applyMove(pieces[i]);
        Realisation newPosition = std::move(*static_cast<Realisation*>(newPositionPtr));
        delete newPositionPtr;
        co_yield new ClassicalHackenbush<Realisation, Piece>(Superposition<Realisation, Piece>(newPosition));
    }
};

#endif // CLASSICAL_HACKENBUSH_H
