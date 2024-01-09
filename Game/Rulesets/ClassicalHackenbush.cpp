#include "ClassicalHackenbush.h"

ClassicalHackenbush::ClassicalHackenbush(const Position* position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> ClassicalHackenbush::getOptions(Player player) const {
    std::vector<QuantumHackenbush*> options;
    std::vector<Edge> pieces = position->getPieces(player);
    for (size_t i = 0; i < pieces.size(); i++) {
        ClassicalPosition *newPosition = position->getRealisation(0).clone();
        newPosition->removePiece(pieces[i]);
        options.push_back(new ClassicalHackenbush(new Position(newPosition)));
    }
    return options;
};

ClassicalHackenbush::~ClassicalHackenbush() {
    delete position;
};
