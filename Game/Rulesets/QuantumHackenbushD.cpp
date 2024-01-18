#include "QuantumHackenbushD.h"

QuantumHackenbushD::QuantumHackenbushD(const Superposition *superposition) : QuantumHackenbush(superposition) {
};

Generator<QuantumHackenbush*> QuantumHackenbushD::options(Player player) const {
    std::vector<Edge> pieces = superposition->getPieces(player);

    // Ruleset D: unsuperposed moves are always allowed
    for (Edge piece : pieces) {
        Superposition *option = new Superposition();
        for (size_t i = 0; i < superposition->getWidth(); i++) {
            Position *newRealisation = superposition->getRealisation(i).applyMove(piece);
            if (newRealisation != nullptr) option->addRealisation(newRealisation);
        }
        if (option->getWidth() > 0) co_yield new QuantumHackenbushD(option);
        else delete option;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushD>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

QuantumHackenbushD::~QuantumHackenbushD() {
    delete superposition;
};
