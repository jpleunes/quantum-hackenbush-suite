#ifndef QUANTUM_HACKENBUSH_D_H
#define QUANTUM_HACKENBUSH_D_H

#include "../QuantumHackenbush.h"

template<typename Piece>
class QuantumHackenbushD : public QuantumHackenbush<Piece> {
public:
    QuantumHackenbushD(const Superposition<Piece>* superposition);
    Generator<QuantumHackenbush<Piece>*> options(Player player) const override;

    ~QuantumHackenbushD() override;
};

// This is a templated class, so the implementations need to go here

template<typename Piece>
QuantumHackenbushD<Piece>::QuantumHackenbushD(const Superposition<Piece>* superposition) : QuantumHackenbush<Piece>(superposition) {
};

template<typename Piece>
Generator<QuantumHackenbush<Piece>*> QuantumHackenbushD<Piece>::options(Player player) const {
    std::vector<Piece> pieces = this->superposition->getPieces(player);

    // Ruleset D: unsuperposed moves are always allowed
    for (Piece piece : pieces) {
        Superposition<Piece> *option = new Superposition<Piece>();
        for (size_t i = 0; i < this->superposition->getWidth(); i++) {
            Position<Piece> *newRealisation = this->superposition->getRealisation(i).applyMove(piece);
            if (newRealisation != nullptr) option->addRealisation(newRealisation);
        }
        if (option->getWidth() > 0) co_yield new QuantumHackenbushD(option);
        else delete option;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushD>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

template<typename Piece>
QuantumHackenbushD<Piece>::~QuantumHackenbushD() {
    delete this->superposition;
};

#endif // QUANTUM_HACKENBUSH_D_H
