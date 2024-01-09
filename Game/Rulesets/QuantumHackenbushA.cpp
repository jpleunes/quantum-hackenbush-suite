#include "QuantumHackenbushA.h"

QuantumHackenbushA::QuantumHackenbushA(const Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushA::getOptions(Player player) const {
    std::vector<QuantumHackenbush*> options;
    std::vector<Edge> pieces = position->getPieces(player);

    auto superposedMoveOptions = getSuperposedMoveOptions<QuantumHackenbushA>(pieces);
    options.insert(options.end(), superposedMoveOptions.begin(), superposedMoveOptions.end());

    return options;
};

QuantumHackenbushA::~QuantumHackenbushA() {
    delete position;
};
