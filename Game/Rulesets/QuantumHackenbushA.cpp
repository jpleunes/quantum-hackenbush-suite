#include "QuantumHackenbushA.h"

QuantumHackenbushA::QuantumHackenbushA(const Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushA::getBlueOptions() const {
    std::vector<QuantumHackenbush*> blueOptions;
    std::vector<Edge> bluePieces = position->getBluePieces();

    auto blueSuperposedMoveOptions = getBlueSuperposedMoveOptions<QuantumHackenbushA>(bluePieces);
    blueOptions.insert(blueOptions.end(), blueSuperposedMoveOptions.begin(), blueSuperposedMoveOptions.end());

    return blueOptions;
};

std::vector<QuantumHackenbush*> QuantumHackenbushA::getRedOptions() const {
    std::vector<QuantumHackenbush*> redOptions;
    std::vector<Edge> redPieces = position->getRedPieces();

    auto redSuperposedMoveOptions = getRedSuperposedMoveOptions<QuantumHackenbushA>(redPieces);
    redOptions.insert(redOptions.end(), redSuperposedMoveOptions.begin(), redSuperposedMoveOptions.end());

    return redOptions;
};

QuantumHackenbushA::~QuantumHackenbushA() {
    delete position;
};
