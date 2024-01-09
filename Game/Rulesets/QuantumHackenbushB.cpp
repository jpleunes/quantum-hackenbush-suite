#include "QuantumHackenbushB.h"

QuantumHackenbushB::QuantumHackenbushB(const Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushB::getOptions(Player player) const {
    std::vector<QuantumHackenbush*> options;
    std::vector<Edge> pieces = position->getPieces(player);

    // Ruleset B: if a player has only one possible move within all realisations together, 
    // he can play it as an unsuperposed move
    if (pieces.size() == 1) {
        Position *option = new Position();
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(pieces[0]);
            if (valid) option->addRealisation(newRealisation);
            else delete newRealisation;
        }
        options.push_back(new QuantumHackenbushB(option));
        return options;
    }

    auto superposedMoveOptions = getSuperposedMoveOptions<QuantumHackenbushB>(pieces);
    options.insert(options.end(), superposedMoveOptions.begin(), superposedMoveOptions.end());

    return options;
};

QuantumHackenbushB::~QuantumHackenbushB() {
    delete position;
};
