#ifndef QUANTUM_HACKENBUSH_C_PRIME_H
#define QUANTUM_HACKENBUSH_C_PRIME_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class QuantumHackenbushCPrime : public QuantumHackenbush<Realisation> {
public:
    QuantumHackenbushCPrime(const Superposition<Realisation> superposition);
    Generator<QuantumHackenbush<Realisation>*> options(Player player) const override;

    ~QuantumHackenbushCPrime() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbushCPrime<Realisation>::QuantumHackenbushCPrime(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
Generator<QuantumHackenbush<Realisation>*> QuantumHackenbushCPrime<Realisation>::options(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);

    // Ruleset C': unsuperposed moves are allowed if and only if they are valid in all possible realisations
    // where he still has at least one classical move
    for (typename Realisation::Piece piece : pieces) {
        Superposition<Realisation> option;
        bool allValidWithClassicalMove = true;
        for (PositionId realisationId : this->superposition.getRealisationIds()) {
            Realisation& realisation = PositionDatabase<Realisation>::getInstance().getGame(realisationId);
            if (realisation.getPieces(player).empty()) continue;
            
            PositionId newRealisationId = realisation.applyMove(piece);
            if (newRealisationId != ILLEGAL_POSITION_ID) option.addRealisationId(newRealisationId);
            else allValidWithClassicalMove = false;
        }
        if (allValidWithClassicalMove && !option.empty()) co_yield new QuantumHackenbushCPrime<Realisation>(option);
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushCPrime<Realisation>>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

#endif // QUANTUM_HACKENBUSH_C_PRIME_H
