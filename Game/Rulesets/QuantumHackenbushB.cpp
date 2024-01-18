#include "QuantumHackenbushB.h"

QuantumHackenbushB::QuantumHackenbushB(const Superposition *superposition) : QuantumHackenbush(superposition) {
};

Generator<QuantumHackenbush*> QuantumHackenbushB::options(Player player) const {
    std::vector<Edge> pieces = superposition->getPieces(player);

    // Ruleset B: if a player has only one possible move within all realisations together, 
    // he can play it as an unsuperposed move
    if (pieces.size() == 1) {
        Superposition *option = new Superposition();
        for (size_t i = 0; i < superposition->getWidth(); i++) {
            Position *newRealisation = superposition->getRealisation(i).applyMove(pieces[0]);
            if (newRealisation != nullptr) option->addRealisation(newRealisation);
        }
        co_yield new QuantumHackenbushB(option);
        co_return;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushB>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

QuantumHackenbushB::~QuantumHackenbushB() {
    delete superposition;
};
