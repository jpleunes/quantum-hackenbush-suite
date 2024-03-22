#include <iostream>

#include "Util/CreatePositionUtil.h"
#include "Game/Rulesets/ClassicalHackenbush.h"
#include "Game/Rulesets/QuantumHackenbushA.h"
#include "Game/Rulesets/QuantumHackenbushB.h"
#include "Game/Rulesets/QuantumHackenbushC.h"
#include "Game/Rulesets/QuantumHackenbushCPrime.h"
#include "Game/Rulesets/QuantumHackenbushD.h"

typedef RestrictedPosition Realisation;

template<typename Ruleset>
void analyse(std::string function, PositionId startId) {
    if (function == "outcome") {
        OutcomeClass outcome = Ruleset(Superposition<Realisation>(startId)).template determineOutcomeClass<Ruleset>();
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
    }
    else if (function == "value") {
        std::optional<DyadicRational> value = Ruleset(Superposition<Realisation>(startId)).template determineValue<Ruleset>();
        if (value.has_value()) std::cout << value.value() << std::endl;
        else std::cout << "NaN" << std::endl; // The value of the starting position is not a number
    }
    else if (function == "birthday") {
        size_t birthday = Ruleset(Superposition<Realisation>(startId)).template determineBirthday<Ruleset>();
        std::cout << birthday << std::endl;
    }
    else throw(std::domain_error("Unknown function."));
}

int main(int argc, char **argv) {
    if (argc < 7) {
        // TODO: support loading standard format from position file
        std::cout << "Usage: qhs <function>[outcome,value,birthday] <nBlueHalfs> <nRedHalfs> <nBlueWholes> <nRedWholes> <ruleset>[classical,a,b,c,cprime,d]" << std::endl;
        return 1;
    }
    std::string function = argv[1];
    size_t nBlueHalfs = std::stoi(argv[2]);
    size_t nRedHalfs = std::stoi(argv[3]);
    size_t nBlueWholes = std::stoi(argv[4]);
    size_t nRedWholes = std::stoi(argv[5]);
    std::string ruleset = argv[6];

    const RestrictedPosition start = createRestrictedPosition(nBlueHalfs, nRedHalfs, nBlueWholes, nRedWholes);
    #ifdef DEBUG
    start->printHumanReadable();
    #endif

    PositionId startId = PositionDatabase<Realisation>::getInstance().getPositionId(start);
    if (ruleset == "classical") analyse<ClassicalHackenbush<Realisation>>(function, startId);
    else if (ruleset == "a") analyse<QuantumHackenbushA<Realisation>>(function, startId);
    else if (ruleset == "b") analyse<QuantumHackenbushB<Realisation>>(function, startId);
    else if (ruleset == "c") analyse<QuantumHackenbushC<Realisation>>(function, startId);
    else if (ruleset == "cprime") analyse<QuantumHackenbushCPrime<Realisation>>(function, startId);
    else if (ruleset == "d") analyse<QuantumHackenbushD<Realisation>>(function, startId);
    else {
        std::cout << "Unknown ruleset" << std::endl;
        return 1;
    }

    return 0;
}
