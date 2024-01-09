#include "QuantumHackenbushD.h"

QuantumHackenbushD::QuantumHackenbushD(const Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushD::getBlueOptions() const {
    std::vector<QuantumHackenbush*> blueOptions;
    std::vector<Edge> bluePieces = position->getBluePieces();

    // Ruleset D: unsuperposed moves are always allowed
    for (Edge piece : bluePieces) {
        Position *option = new Position();
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(piece);
            if (valid) option->addRealisation(newRealisation);
            else delete newRealisation;
        }
        if (option->getWidth() > 0) blueOptions.push_back(new QuantumHackenbushD(option));
        else delete option;
    }

    auto blueSuperposedMoveOptions = getBlueSuperposedMoveOptions<QuantumHackenbushD>(bluePieces);
    blueOptions.insert(blueOptions.end(), blueSuperposedMoveOptions.begin(), blueSuperposedMoveOptions.end());

    return blueOptions;
};

std::vector<QuantumHackenbush*> QuantumHackenbushD::getRedOptions() const {
    std::vector<QuantumHackenbush*> redOptions;
    std::vector<Edge> redPieces = position->getRedPieces();

    // Ruleset D: unsuperposed moves are always allowed
    for (Edge piece : redPieces) {
        Position *option = new Position();
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(piece);
            if (valid) option->addRealisation(newRealisation);
            else delete newRealisation;
        }
        if (option->getWidth() > 0) redOptions.push_back(new QuantumHackenbushD(option));
        else delete option;
    }

    auto redSuperposedMoveOptions = getRedSuperposedMoveOptions<QuantumHackenbushD>(redPieces);
    redOptions.insert(redOptions.end(), redSuperposedMoveOptions.begin(), redSuperposedMoveOptions.end());

    return redOptions;
};

QuantumHackenbushD::~QuantumHackenbushD() {
    delete position;
};
