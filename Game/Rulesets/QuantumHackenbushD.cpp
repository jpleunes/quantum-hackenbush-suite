#include "QuantumHackenbushD.h"

QuantumHackenbushD::QuantumHackenbushD(const Position *position) : QuantumHackenbush(position) {
};

Generator<QuantumHackenbush*> QuantumHackenbushD::options(Player player) const {
    std::vector<Edge> pieces = position->getPieces(player);

    // Ruleset D: unsuperposed moves are always allowed
    for (Edge piece : pieces) {
        Position *option = new Position();
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(piece);
            if (valid) option->addRealisation(newRealisation);
            else delete newRealisation;
        }
        if (option->getWidth() > 0) co_yield new QuantumHackenbushD(option);
        else delete option;
    }

    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushD>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

QuantumHackenbushD::~QuantumHackenbushD() {
    delete position;
};
