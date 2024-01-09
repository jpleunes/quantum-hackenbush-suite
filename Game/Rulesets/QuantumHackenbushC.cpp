#include "QuantumHackenbushC.h"

QuantumHackenbushC::QuantumHackenbushC(const Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushC::getOptions(Player player) const {
    std::vector<QuantumHackenbush*> options;
    std::vector<Edge> pieces = position->getPieces(player);

    // Ruleset C: unsuperposed moves are allowed if and only if they are valid in all possible realisations
    for (Edge piece : pieces) {
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
        if (allValid && option->getWidth() > 0) options.push_back(new QuantumHackenbushC(option));
        else delete option;
    }

    auto superposedMoveOptions = getSuperposedMoveOptions<QuantumHackenbushC>(pieces);
    options.insert(options.end(), superposedMoveOptions.begin(), superposedMoveOptions.end());

    return options;
};

QuantumHackenbushC::~QuantumHackenbushC() {
    delete position;
};
