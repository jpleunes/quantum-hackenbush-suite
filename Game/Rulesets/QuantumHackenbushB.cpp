#include "QuantumHackenbushB.h"

QuantumHackenbushB::QuantumHackenbushB(Position *position) : QuantumHackenbush(position) {
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

    if (bluePieces.size() < width) return blueOptions;

    for (std::vector<size_t> move : indexCombinations(bluePieces.size())) {
        Position *option = new Position();
        for (size_t i = 0; i < position->getWidth(); i++) {
            for (size_t pieceIndex : move) {
                ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                bool valid = newRealisation->removePiece(bluePieces[pieceIndex]);
                if (valid) option->addRealisation(newRealisation);
                else delete newRealisation;
            }
        }
        if (option->getWidth() > 0) blueOptions.push_back(new QuantumHackenbushB(option));
        else delete option;
    }
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

    if (redPieces.size() < width) return redOptions;

    for (std::vector<size_t> move : indexCombinations(redPieces.size())) {
        Position *option = new Position();
        for (size_t i = 0; i < position->getWidth(); i++) {
            for (size_t pieceIndex : move) {
                ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                bool valid = newRealisation->removePiece(redPieces[pieceIndex]);
                if (valid) option->addRealisation(newRealisation);
                else delete newRealisation;
            }
        }
        if (option->getWidth() > 0) redOptions.push_back(new QuantumHackenbushB(option));
        else delete option;
    }
    return redOptions;
};

QuantumHackenbushB::~QuantumHackenbushB() {
    delete position;
};
