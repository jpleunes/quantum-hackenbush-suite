#include <iostream>

#include "Util/CreatePositionUtil.h"
#include "Game/Flavours/SuperpositionClassical.h"
#include "Game/Flavours/SuperpositionA.h"
#include "Game/Flavours/SuperpositionB.h"
#include "Game/Flavours/SuperpositionC.h"
#include "Game/Flavours/SuperpositionCPrime.h"
#include "Game/Flavours/SuperpositionD.h"

typedef RestrictedPosition Realisation;

template<typename Flavour>
void analyse(std::string function, PositionId startId) {
    if (function == "outcome") {
        OutcomeClass outcome = Flavour(startId).template determineOutcomeClass<Flavour>();
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
        std::optional<DyadicRational> value = Flavour(startId).template determineValue<Flavour>();
        if (value.has_value()) std::cout << value.value() << std::endl;
        else std::cout << "NaN" << std::endl; // The value of the starting position is not a number
    }
    else if (function == "birthday") {
        size_t birthday = Flavour(startId).template determineBirthday<Flavour>();
        std::cout << birthday << std::endl;
    }
    else throw(std::domain_error("Unknown function."));
}

int main(int argc, char **argv) {
    if (argc < 7) {
        // TODO: support loading standard format from position file
        std::cout << "Usage: qhs <function>[outcome,value,birthday] <nBlueHalfs> <nRedHalfs> <nBlueWholes> <nRedWholes> <flavour>[classical,a,b,c,cprime,d]" << std::endl;
        return 1;
    }
    std::string function = argv[1];
    size_t nBlueHalfs = std::stoi(argv[2]);
    size_t nRedHalfs = std::stoi(argv[3]);
    size_t nBlueWholes = std::stoi(argv[4]);
    size_t nRedWholes = std::stoi(argv[5]);
    std::string flavour = argv[6];

    const RestrictedPosition start = createRestrictedPosition(nBlueHalfs, nRedHalfs, nBlueWholes, nRedWholes);
    #ifdef DEBUG
    start->printHumanReadable();
    #endif

    PositionId startId = PositionDatabase<Realisation>::getInstance().getPositionId(start);
    if (flavour == "classical") analyse<SuperpositionClassical<Realisation>>(function, startId);
    else if (flavour == "a") analyse<SuperpositionA<Realisation>>(function, startId);
    else if (flavour == "b") analyse<SuperpositionB<Realisation>>(function, startId);
    else if (flavour == "c") analyse<SuperpositionC<Realisation>>(function, startId);
    else if (flavour == "cprime") analyse<SuperpositionCPrime<Realisation>>(function, startId);
    else if (flavour == "d") analyse<SuperpositionD<Realisation>>(function, startId);
    else {
        std::cout << "Unknown flavour" << std::endl;
        return 1;
    }

    return 0;
}
