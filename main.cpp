#include <iostream>

#include "Util/PositionUtil.h"
#include "Game/Rulesets/ClassicalHackenbush.h"
#include "Game/Rulesets/QuantumHackenbushA.h"

int main(int argc, char **argv) {
    if (argc < 6) {
        // TODO: support loading standard format from position file
        std::cout << "Usage: qhs <nBlueHalfs> <nRedHalfs> <nBlueWholes> <nRedWholes> <ruleset>" << std::endl;
        return 1;
    }
    int nBlueHalfs = std::stoi(argv[1]);
    int nRedHalfs = std::stoi(argv[2]);
    int nBlueWholes = std::stoi(argv[3]);
    int nRedWholes = std::stoi(argv[4]);
    std::string ruleset = argv[5]; // TODO: actually use ruleset

    AdjacencyMatrixPosition *start = createRestrictedPosition(1 + 2 * nBlueHalfs + 2 * nRedHalfs + nBlueWholes + nRedWholes, nBlueHalfs, nRedHalfs, nBlueWholes, nRedWholes);

    QuantumHackenbush* game;
    Position* position = NULL;
    if (ruleset == "classical") {
        game = new ClassicalHackenbush(start);
    }
    else if (ruleset == "a") {
        position = new Position(start);
        game = new QuantumHackenbushA(position);
    }
    else {
        std::cout << "Unknown ruleset" << std::endl;
        return 1;
    }
    std::cout << "Outcome class:" << std::endl;
    switch (game->determineOutcomeClass())
    {
        case OutcomeClass::L:
            std::cout << "L-position" << std::endl;
            break;
        case OutcomeClass::R:
            std::cout << "R-position" << std::endl;
            break;
        case OutcomeClass::N:
            std::cout << "N-position" << std::endl;
            break;
        case OutcomeClass::P:
            std::cout << "P-position" << std::endl;
            break;
        default: std::cout << "error" << std::endl;
    }

    delete game;
    if (position != NULL) delete position;
    return 0;
}
