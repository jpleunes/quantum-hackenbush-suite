#ifndef CLASSICAL_HACKENBUSH_H
#define CLASSICAL_HACKENBUSH_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class ClassicalHackenbush : public QuantumHackenbush<Realisation> {
public:
    ClassicalHackenbush(const Superposition<Realisation> superposition);
    Generator<QuantumHackenbush<Realisation>*> options(Player player) const override;

    ~ClassicalHackenbush() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
ClassicalHackenbush<Realisation>::ClassicalHackenbush(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
Generator<QuantumHackenbush<Realisation>*> ClassicalHackenbush<Realisation>::options(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);
    for (size_t i = 0; i < pieces.size(); i++) {
        Realisation& position = PositionDatabase<Realisation>::getInstance().getGame(*this->superposition.getRealisationIds().begin());
        PositionId newPositionId = position.applyMove(pieces[i]);
        co_yield new ClassicalHackenbush<Realisation>(Superposition<Realisation>(newPositionId));
    }
};

#endif // CLASSICAL_HACKENBUSH_H
