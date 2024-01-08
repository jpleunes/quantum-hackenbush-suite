#include "QuantumHackenbushC.h"

QuantumHackenbushC::QuantumHackenbushC(const Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushC::getBlueOptions() const {
    std::vector<QuantumHackenbush*> blueOptions;
    std::vector<Edge> bluePieces = position->getBluePieces();

    // Ruleset C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    for (Edge piece : bluePieces) {
        Position *option = new Position();
        bool allValid = true;
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(piece);
            if (valid) option->addRealisation(newRealisation);
            else {
                allValid = false;
                delete newRealisation;
            }
        }
        if (allValid && option->getWidth() > 0) blueOptions.push_back(new QuantumHackenbushC(option));
        else delete option;
    }

    auto blueSuperposedMoveOptions = getBlueSuperposedMoveOptions<QuantumHackenbushC>(bluePieces);
    blueOptions.insert(blueOptions.end(), blueSuperposedMoveOptions.begin(), blueSuperposedMoveOptions.end());

    return blueOptions;
};

std::vector<QuantumHackenbush*> QuantumHackenbushC::getRedOptions() const {
    std::vector<QuantumHackenbush*> redOptions;
    std::vector<Edge> redPieces = position->getRedPieces();

    // Ruleset C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    for (Edge piece : redPieces) {
        Position *option = new Position();
        bool allValid = true;
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(piece);
            if (valid) option->addRealisation(newRealisation);
            else {
                allValid = false;
                delete newRealisation;
            }
        }
        if (allValid && option->getWidth() > 0) redOptions.push_back(new QuantumHackenbushC(option));
        else delete option;
    }

    auto redSuperposedMoveOptions = getRedSuperposedMoveOptions<QuantumHackenbushC>(redPieces);
    redOptions.insert(redOptions.end(), redSuperposedMoveOptions.begin(), redSuperposedMoveOptions.end());

    return redOptions;
};

QuantumHackenbushC::~QuantumHackenbushC() {
    delete position;
};
