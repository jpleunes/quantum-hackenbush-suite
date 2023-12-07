#include <iostream>

#include "Game/Rulesets/ClassicalHackenbush.h"
#include "Util/PositionUtil.h"

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

    std::cout << "Outcome class:" << std::endl;
    ClassicalHackenbush game(start);
    switch (game.determineOutcomeClass())
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

    return 0;
}
