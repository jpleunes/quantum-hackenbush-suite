#include "QuantumHackenbushC.h"

QuantumHackenbushC::QuantumHackenbushC(const Superposition *superposition) : QuantumHackenbush(superposition) {
};

Generator<QuantumHackenbush*> QuantumHackenbushC::options(Player player) const {
    std::vector<Edge> pieces = superposition->getPieces(player);

    // Ruleset C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    for (Edge piece : pieces) {
        Superposition *option = new Superposition();
        bool allValid = true;
        for (size_t i = 0; i < superposition->getWidth(); i++) {
            Position *newRealisation = superposition->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(piece);
            if (valid) option->addRealisation(newRealisation);
            else {
                allValid = false;
                delete newRealisation;
            }
        }
        if (allValid && option->getWidth() > 0) co_yield new QuantumHackenbushC(option);
        else delete option;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushC>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

QuantumHackenbushC::~QuantumHackenbushC() {
    delete superposition;
};
