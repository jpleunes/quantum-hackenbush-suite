#include <iostream>

#include "Flavours/SuperposedGameState/GameState/GameState.h"
#include "Flavours/SuperposedGameState/SuperposedGameState.h"
#include "Util/CreatePositionUtil.h"
#include "Flavours/SuperposedGameStateClassical.h"
#include "Flavours/SuperposedGameStateA.h"
#include "Flavours/SuperposedGameStateB.h"
#include "Flavours/SuperposedGameStateC.h"
#include "Flavours/SuperposedGameStateCPrime.h"
#include "Flavours/SuperposedGameStateD.h"

typedef RestrictedPosition Realisation;

template<typename Flavour>
void analyse(std::string function, const Realisation& start) {
    GameState<Realisation> startGameState = GameStateDatabase<Realisation>::getInstance().getOrInsert(start);
    Flavour startSuperposedGameState = SuperposedGameStateDatabase<Flavour>::getInstance().getOrInsert(startGameState.getId());
    ShortGame& shortGame = ShortGameDatabase::getInstance().getGame(startSuperposedGameState.template determineShortGameId<Flavour>());

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

    if (flavour == "classical") analyse<SuperposedGameStateClassical<Realisation>>(function, start);
    else if (flavour == "a") analyse<SuperposedGameStateA<Realisation>>(function, start);
    else if (flavour == "b") analyse<SuperposedGameStateB<Realisation>>(function, start);
    else if (flavour == "c") analyse<SuperposedGameStateC<Realisation>>(function, start);
    else if (flavour == "cprime") analyse<SuperposedGameStateCPrime<Realisation>>(function, start);
    else if (flavour == "d") analyse<SuperposedGameStateD<Realisation>>(function, start);
    else {
        std::cout << "Unknown flavour" << std::endl;
        return 1;
    }

    return 0;
}
