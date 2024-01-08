#include "ClassicalHackenbush.h"

ClassicalHackenbush::ClassicalHackenbush(const Position* position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> ClassicalHackenbush::getBlueOptions() const {
    std::vector<QuantumHackenbush*> blueOptions;
    std::vector<Edge> bluePieces = position->getBluePieces();
    for (size_t i = 0; i < bluePieces.size(); i++) {
        ClassicalPosition *newPosition = position->getRealisation(0).clone();
        newPosition->removePiece(bluePieces[i]);
        blueOptions.push_back(new ClassicalHackenbush(new Position(newPosition)));
    }
    return blueOptions;
};

std::vector<QuantumHackenbush*> ClassicalHackenbush::getRedOptions() const {
    std::vector<QuantumHackenbush*> redOptions;
    std::vector<Edge> redPieces = position->getRedPieces();
    for (size_t i = 0; i < redPieces.size(); i++) {
        ClassicalPosition *newPosition = position->getRealisation(0).clone();
        newPosition->removePiece(redPieces[i]);
        redOptions.push_back(new ClassicalHackenbush(new Position(newPosition)));
    }
    return redOptions;
};

ClassicalHackenbush::~ClassicalHackenbush() {
    delete position;
};
