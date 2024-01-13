#include "QuantumHackenbushA.h"

QuantumHackenbushA::QuantumHackenbushA(const Position *position) : QuantumHackenbush(position) {
};

Generator<QuantumHackenbush*> QuantumHackenbushA::options(Player player) const {
    std::vector<Edge> pieces = position->getPieces(player);
    auto superposedMoveOptionsGen = superposedMoveOptions<QuantumHackenbushA>(pieces);
    while (superposedMoveOptionsGen) co_yield superposedMoveOptionsGen();
};

QuantumHackenbushA::~QuantumHackenbushA() {
    delete position;
};
