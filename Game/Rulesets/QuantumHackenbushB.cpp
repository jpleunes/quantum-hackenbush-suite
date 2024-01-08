#include "QuantumHackenbushB.h"

QuantumHackenbushB::QuantumHackenbushB(const Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushB::getBlueOptions() const {
    std::vector<QuantumHackenbush*> blueOptions;
    std::vector<Edge> bluePieces = position->getBluePieces();

    // Ruleset B: if a player has only one possible move within all realisations together, 
    // he can play it as an unsuperposed move
    if (bluePieces.size() == 1) {
        Position *option = new Position();
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(bluePieces[0]);
            if (valid) option->addRealisation(newRealisation);
            else delete newRealisation;
        }
        blueOptions.push_back(new QuantumHackenbushB(option));
        return blueOptions;
    }

    auto blueSuperposedMoveOptions = getBlueSuperposedMoveOptions<QuantumHackenbushB>(bluePieces);
    blueOptions.insert(blueOptions.end(), blueSuperposedMoveOptions.begin(), blueSuperposedMoveOptions.end());

    return blueOptions;
};

std::vector<QuantumHackenbush*> QuantumHackenbushB::getRedOptions() const {
    std::vector<QuantumHackenbush*> redOptions;
    std::vector<Edge> redPieces = position->getRedPieces();

    // Ruleset B: if a player has only one possible move within all realisations together, 
    // he can play it as an unsuperposed move
    if (redPieces.size() == 1) {
        Position *option = new Position();
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(redPieces[0]);
            if (valid) option->addRealisation(newRealisation);
            else delete newRealisation;
        }
        redOptions.push_back(new QuantumHackenbushB(option));
        return redOptions;
    }

    auto redSuperposedMoveOptions = getRedSuperposedMoveOptions<QuantumHackenbushB>(redPieces);
    redOptions.insert(redOptions.end(), redSuperposedMoveOptions.begin(), redSuperposedMoveOptions.end());

    return redOptions;
};

QuantumHackenbushB::~QuantumHackenbushB() {
    delete position;
};
