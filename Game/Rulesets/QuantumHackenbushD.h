#ifndef QUANTUM_HACKENBUSH_D_H
#define QUANTUM_HACKENBUSH_D_H

#include "../QuantumHackenbush.h"

template<typename Realisation, typename Piece>
class QuantumHackenbushD : public QuantumHackenbush<Realisation, Piece> {
public:
    QuantumHackenbushD(const Superposition<Realisation, Piece> superposition);
    Generator<QuantumHackenbush<Realisation, Piece>*> options(Player player) const override;

    ~QuantumHackenbushD() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation, typename Piece>
QuantumHackenbushD<Realisation, Piece>::QuantumHackenbushD(const Superposition<Realisation, Piece> superposition) : QuantumHackenbush<Realisation, Piece>(superposition) {
};

template<typename Realisation, typename Piece>
Generator<QuantumHackenbush<Realisation, Piece>*> QuantumHackenbushD<Realisation, Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition.getPieces(player);

    // Ruleset D: unsuperposed moves are always allowed
    for (Piece piece : pieces) {
        Superposition<Realisation, Piece> option;
        for (const Realisation* realisation : this->superposition.getRealisations()) {
            auto* newRealisationPtr = realisation->applyMove(piece);
            if (newRealisationPtr != nullptr) {
                Realisation newRealisation = std::move(*static_cast<Realisation*>(newRealisationPtr));
                delete newRealisationPtr;
                option.addRealisation(newRealisation);
            }
        }
        if (!option.empty()) co_yield new QuantumHackenbushD(option);
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushD>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

#endif // QUANTUM_HACKENBUSH_D_H
