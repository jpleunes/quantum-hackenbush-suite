#include "ClassicalHackenbush.h"

ClassicalHackenbush::ClassicalHackenbush(const Position* position) : QuantumHackenbush(position) {
};

Generator<QuantumHackenbush*> ClassicalHackenbush::options(Player player) const {
    std::vector<Edge> pieces = position->getPieces(player);
    for (size_t i = 0; i < pieces.size(); i++) {
        ClassicalPosition *newPosition = position->getRealisation(0).clone();
        newPosition->removePiece(pieces[i]);
        co_yield new ClassicalHackenbush(new Position(newPosition));
    }
};

ClassicalHackenbush::~ClassicalHackenbush() {
    delete position;
};
