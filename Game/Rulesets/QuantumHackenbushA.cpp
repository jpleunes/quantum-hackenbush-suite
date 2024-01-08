#include "QuantumHackenbushA.h"

QuantumHackenbushA::QuantumHackenbushA(Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushA::getBlueOptions() const {
    std::vector<QuantumHackenbush*> blueOptions;
    std::vector<Edge> bluePieces = position->getBluePieces();

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
        if (option->getWidth() > 0) blueOptions.push_back(new QuantumHackenbushA(option));
        else delete option;
    }
    return blueOptions;
};

std::vector<QuantumHackenbush*> QuantumHackenbushA::getRedOptions() const {
    std::vector<QuantumHackenbush*> redOptions;
    std::vector<Edge> redPieces = position->getRedPieces();

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
        if (option->getWidth() > 0) redOptions.push_back(new QuantumHackenbushA(option));
        else delete option;
    }
    return redOptions;
};

QuantumHackenbushA::~QuantumHackenbushA() {
    delete position;
};
