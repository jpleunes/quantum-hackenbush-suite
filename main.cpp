#include <iostream>

#include "Util/CreatePositionUtil.h"
#include "Game/Rulesets/ClassicalHackenbush.h"
#include "Game/Rulesets/QuantumHackenbushA.h"
#include "Game/Rulesets/QuantumHackenbushB.h"
#include "Game/Rulesets/QuantumHackenbushC.h"
#include "Game/Rulesets/QuantumHackenbushCPrime.h"
#include "Game/Rulesets/QuantumHackenbushD.h"

typedef RestrictedPosition PositionType;

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

    const RestrictedPosition start = createRestrictedPosition(nBlueHalfs, nRedHalfs, nBlueWholes, nRedWholes);
    #ifdef DEBUG
    start->printHumanReadable();
    #endif

    PositionId startId = PositionDatabase<PositionType>::getInstance().getPositionId(start);
    OutcomeClass outcome;
    if (ruleset == "classical") {
        outcome = ClassicalHackenbush<PositionType>(Superposition<PositionType>(startId)).determineOutcomeClass<ClassicalHackenbush<PositionType>>();
    }
    else if (ruleset == "a") {
        outcome = QuantumHackenbushA<PositionType>(Superposition<PositionType>(startId)).determineOutcomeClass<QuantumHackenbushA<PositionType>>();
    }
    else if (ruleset == "b") {
        outcome = QuantumHackenbushB<PositionType>(Superposition<PositionType>(startId)).determineOutcomeClass<QuantumHackenbushB<PositionType>>();
    }
    else if (ruleset == "c") {
        outcome = QuantumHackenbushC<PositionType>(Superposition<PositionType>(startId)).determineOutcomeClass<QuantumHackenbushC<PositionType>>();
    }
    else if (ruleset == "cprime") {
        outcome = QuantumHackenbushCPrime<PositionType>(Superposition<PositionType>(startId)).determineOutcomeClass<QuantumHackenbushCPrime<PositionType>>();
    }
    else if (ruleset == "d") {
        outcome = QuantumHackenbushD<PositionType>(Superposition<PositionType>(startId)).determineOutcomeClass<QuantumHackenbushD<PositionType>>();
    }
    else {
        std::cout << "Unknown ruleset" << std::endl;
        return 1;
    }
    switch (outcome)
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

    return 0;
}
