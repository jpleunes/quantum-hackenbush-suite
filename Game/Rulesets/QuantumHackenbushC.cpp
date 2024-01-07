#include "QuantumHackenbushC.h"

QuantumHackenbushC::QuantumHackenbushC(Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushC::getBlueOptions() const {
    std::vector<QuantumHackenbush*> blueOptions;
    std::vector<Edge> bluePieces = position->getBluePieces();

    // Ruleset C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    for (Edge piece : bluePieces) {
        Position *option = new Position(position->getRealisation(0).getNodeCount()); // TODO: find number of possible nodes (nodes may have been deleted in some realisations)
        bool allValid = true;
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(piece);
            if (valid) option->addRealisation(newRealisation);
            else allValid = false;
        }
        if (allValid && option->getWidth() > 0) blueOptions.push_back(new QuantumHackenbushC(option));
        else delete option;
    }

    if (bluePieces.size() < width) return blueOptions;

    for (std::vector<size_t> move : indexCombinations(bluePieces.size())) {
        Position *option = new Position(position->getRealisation(0).getNodeCount()); // TODO: find number of possible nodes (nodes may have been deleted in some realisations)
        for (size_t i = 0; i < position->getWidth(); i++) {
            for (size_t pieceIndex : move) {
                ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                bool valid = newRealisation->removePiece(bluePieces[pieceIndex]);
                if (valid) option->addRealisation(newRealisation);
            }
        }
        if (option->getWidth() > 0) blueOptions.push_back(new QuantumHackenbushC(option));
        else delete option;
    }
    return blueOptions;
};

std::vector<QuantumHackenbush*> QuantumHackenbushC::getRedOptions() const {
    std::vector<QuantumHackenbush*> redOptions;
    std::vector<Edge> redPieces = position->getRedPieces();

    // Ruleset C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    for (Edge piece : redPieces) {
        Position *option = new Position(position->getRealisation(0).getNodeCount()); // TODO: find number of possible nodes (nodes may have been deleted in some realisations)
        bool allValid = true;
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(piece);
            if (valid) option->addRealisation(newRealisation);
            else allValid = false;
        }
        if (allValid && option->getWidth() > 0) redOptions.push_back(new QuantumHackenbushC(option));
        else delete option;
    }

    if (redPieces.size() < width) return redOptions;

    for (std::vector<size_t> move : indexCombinations(redPieces.size())) {
        Position *option = new Position(position->getRealisation(0).getNodeCount()); // TODO: find number of possible nodes (nodes may have been deleted in some realisations)
        for (size_t i = 0; i < position->getWidth(); i++) {
            for (size_t pieceIndex : move) {
                ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                bool valid = newRealisation->removePiece(redPieces[pieceIndex]);
                if (valid) option->addRealisation(newRealisation);
            }
        }
        if (option->getWidth() > 0) redOptions.push_back(new QuantumHackenbushC(option));
        else delete option;
    }
    return redOptions;
};

QuantumHackenbushC::~QuantumHackenbushC() {
    delete position;
};
