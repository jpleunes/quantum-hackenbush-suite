#include "QuantumHackenbushB.h"

QuantumHackenbushB::QuantumHackenbushB(const Position *position) : QuantumHackenbush(position) {
};

Generator<QuantumHackenbush*> QuantumHackenbushB::options(Player player) const {
    std::vector<Edge> pieces = position->getPieces(player);

    // Ruleset B: if a player has only one possible move within all realisations together, 
    // he can play it as an unsuperposed move
    if (pieces.size() == 1) {
        Position *option = new Position();
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(pieces[0]);
            if (valid) option->addRealisation(newRealisation);
            else delete newRealisation;
        }
        co_yield new QuantumHackenbushB(option);
        co_return;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushB>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

QuantumHackenbushB::~QuantumHackenbushB() {
    delete position;
};
