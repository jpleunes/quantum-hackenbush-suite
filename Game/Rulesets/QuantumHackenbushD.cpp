#include "QuantumHackenbushD.h"

QuantumHackenbushD::QuantumHackenbushD(const Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushD::getOptions(Player player) const {
    std::vector<QuantumHackenbush*> options;
    std::vector<Edge> pieces = position->getPieces(player);

    // Ruleset D: unsuperposed moves are always allowed
    for (Edge piece : pieces) {
        Position *option = new Position();
        for (size_t i = 0; i < position->getWidth(); i++) {
            ClassicalPosition *newRealisation = position->getRealisation(i).clone();
            bool valid = newRealisation->removePiece(piece);
            if (valid) option->addRealisation(newRealisation);
            else delete newRealisation;
        }
        if (option->getWidth() > 0) options.push_back(new QuantumHackenbushD(option));
        else delete option;
    }

    auto superposedMoveOptions = getSuperposedMoveOptions<QuantumHackenbushD>(pieces);
    options.insert(options.end(), superposedMoveOptions.begin(), superposedMoveOptions.end());

    return options;
};

QuantumHackenbushD::~QuantumHackenbushD() {
    delete position;
};
