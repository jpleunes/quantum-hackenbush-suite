#include "QuantumHackenbushA.h"

QuantumHackenbushA::QuantumHackenbushA(const Superposition *superposition) : QuantumHackenbush(superposition) {
};

Generator<QuantumHackenbush*> QuantumHackenbushA::options(Player player) const {
    std::vector<Edge> pieces = superposition->getPieces(player);
    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushA>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

QuantumHackenbushA::~QuantumHackenbushA() {
    delete superposition;
};
