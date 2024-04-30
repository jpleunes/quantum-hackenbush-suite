#include <iostream>

#include "Flavours/Superposition/Position/Hackenbush.h"
#include "Flavours/Superposition/Superposition.h"
#include "Util/CreatePositionUtil.h"
#include "Flavours/SuperpositionClassical.h"
#include "Flavours/SuperpositionA.h"
#include "Flavours/SuperpositionB.h"
#include "Flavours/SuperpositionC.h"
#include "Flavours/SuperpositionCPrime.h"
#include "Flavours/SuperpositionD.h"

typedef RestrictedPosition Realisation;

template<typename Flavour>
void analyse(std::string function, const Realisation& start) {
    Hackenbush<Realisation> startHackenbush = HackenbushDatabase<Realisation>::getInstance().getOrInsert(start);
    Flavour startSuperposition = SuperpositionDatabase<Flavour>::getInstance().getOrInsert(startHackenbush.getId());
    ShortGame& shortGame = ShortGameDatabase::getInstance().getGame(startSuperposition.template determineShortGameId<Flavour>());

    if (function == "outcome") {
        OutcomeClass outcome = shortGame.determineOutcomeClass();
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
        std::optional<DyadicRational> value = shortGame.determineNumberValue();
        if (value.has_value()) std::cout << value.value() << std::endl;
        else std::cout << "NaN" << std::endl; // The value of the starting position is not a number
    }
    else if (function == "birthday") {
        size_t birthday = shortGame.determineBirthday();
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

    if (flavour == "classical") analyse<SuperpositionClassical<Realisation>>(function, start);
    else if (flavour == "a") analyse<SuperpositionA<Realisation>>(function, start);
    else if (flavour == "b") analyse<SuperpositionB<Realisation>>(function, start);
    else if (flavour == "c") analyse<SuperpositionC<Realisation>>(function, start);
    else if (flavour == "cprime") analyse<SuperpositionCPrime<Realisation>>(function, start);
    else if (flavour == "d") analyse<SuperpositionD<Realisation>>(function, start);
    else {
        std::cout << "Unknown flavour" << std::endl;
        return 1;
    }

    return 0;
}
