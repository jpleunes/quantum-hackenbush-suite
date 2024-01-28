#include <iostream>

#include "Util/PositionUtil.h"
#include "Game/Rulesets/ClassicalHackenbush.h"
#include "Game/Rulesets/QuantumHackenbushA.h"
#include "Game/Rulesets/QuantumHackenbushB.h"
#include "Game/Rulesets/QuantumHackenbushC.h"
#include "Game/Rulesets/QuantumHackenbushCPrime.h"
#include "Game/Rulesets/QuantumHackenbushD.h"

int main(int argc, char **argv) {
    if (argc < 6) {
        // TODO: support loading standard format from position file
        std::cout << "Usage: qhs <nBlueHalfs> <nRedHalfs> <nBlueWholes> <nRedWholes> <ruleset>" << std::endl;
        return 1;
    }
    size_t nBlueHalfs = std::stoi(argv[1]);
    size_t nRedHalfs = std::stoi(argv[2]);
    size_t nBlueWholes = std::stoi(argv[3]);
    size_t nRedWholes = std::stoi(argv[4]);
    std::string ruleset = argv[5];

    const RestrictedPosition *start = createRestrictedPosition(nBlueHalfs, nRedHalfs, nBlueWholes, nRedWholes);
    #ifdef DEBUG
    start->printHumanReadable();
    #endif

    QuantumHackenbush<RestrictedPiece>* game;
    if (ruleset == "classical") {
        game = new ClassicalHackenbush(new Superposition(start));
    }
    else if (ruleset == "a") {
        game = new QuantumHackenbushA(new Superposition(start));
    }
    else if (ruleset == "b") {
        game = new QuantumHackenbushB(new Superposition(start));
    }
    else if (ruleset == "c") {
        game = new QuantumHackenbushC(new Superposition(start));
    }
    else if (ruleset == "cprime") {
        game = new QuantumHackenbushCPrime(new Superposition(start));
    }
    else if (ruleset == "d") {
        game = new QuantumHackenbushD(new Superposition(start));
    }
    else {
        std::cout << "Unknown ruleset" << std::endl;
        return 1;
    }
    switch (game->determineOutcomeClass())
    {
        case OutcomeClass::L:
            std::cout << "L" << std::endl;
            break;
        case OutcomeClass::R:
            std::cout << "R" << std::endl;
            break;
        case OutcomeClass::N:
            std::cout << "N" << std::endl;
            break;
        case OutcomeClass::P:
            std::cout << "P" << std::endl;
            break;
        default: std::cout << "error" << std::endl;
    }

    delete game;
    return 0;
}
