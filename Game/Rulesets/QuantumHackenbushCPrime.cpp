#include "QuantumHackenbushCPrime.h"

QuantumHackenbushCPrime::QuantumHackenbushCPrime(const Position *position) : QuantumHackenbush(position) {
};

Generator<QuantumHackenbush*> QuantumHackenbushCPrime::options(Player player) const {
    std::vector<Edge> pieces = position->getPieces(player);

    // Ruleset C': unsuperposed moves are allowed if and only if they are valid in all possible realisations
    // where he still has at least one classical move
    for (Edge piece : pieces) {
        Position *option = new Position();
        bool allValidWithClassicalMove = true;
        for (size_t i = 0; i < position->getWidth(); i++) {
            if (position->getRealisation(i).getPieces(player).empty()) continue;
            
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(piece);
            if (valid) option->addRealisation(newRealisation);
            else {
                allValidWithClassicalMove = false;
                delete newRealisation;
            }
        }
        if (allValidWithClassicalMove && option->getWidth() > 0) co_yield new QuantumHackenbushCPrime(option);
        else delete option;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushCPrime>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

QuantumHackenbushCPrime::~QuantumHackenbushCPrime() {
    delete position;
};
