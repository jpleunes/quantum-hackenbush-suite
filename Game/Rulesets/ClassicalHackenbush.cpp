#include "ClassicalHackenbush.h"

ClassicalHackenbush::ClassicalHackenbush(const Superposition *superposition) : QuantumHackenbush(superposition) {
};

Generator<QuantumHackenbush*> ClassicalHackenbush::options(Player player) const {
    std::vector<Edge> pieces = superposition->getPieces(player);
    for (size_t i = 0; i < pieces.size(); i++) {
        Position *newPosition = superposition->getRealisation(0).clone();
        newPosition->removePiece(pieces[i]);
        co_yield new ClassicalHackenbush(new Superposition(newPosition));
    }
};

ClassicalHackenbush::~ClassicalHackenbush() {
    delete superposition;
};
