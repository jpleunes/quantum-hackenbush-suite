#include <iostream>
#include <sstream>

#include "Flavours/SuperposedGameState/GameState/GameState.h"
#include "Flavours/SuperposedGameState/SuperposedGameState.h"
#include "Util/CreatePositionUtil.h"
#include "Flavours/SuperposedGameStateClassical.h"
#include "Flavours/SuperposedGameStateA.h"
#include "Flavours/SuperposedGameStateB.h"
#include "Flavours/SuperposedGameStateC.h"
#include "Flavours/SuperposedGameStateCPrime.h"
#include "Flavours/SuperposedGameStateD.h"

// By default, allow superposed moves of unbounded width
size_t width = std::numeric_limits<size_t>::max();

template<typename Flavour>
ShortGameId gameStateIdToShortGameId(GameStateId gameStateId) {
    Flavour startSuperposedGameState = SuperposedGameStateDatabase<Flavour>::getInstance().getOrInsert(gameStateId);
    return startSuperposedGameState.template determineShortGameId<Flavour>(width);
}

template<typename Realisation>
void analyse(std::string function, GameStateId startGameState, std::string flavour) {
    ShortGameId shortGameId = -1;
    if (flavour == "classical") shortGameId = gameStateIdToShortGameId<SuperposedGameStateClassical<Realisation>>(startGameState);
    else if (flavour == "a") shortGameId = gameStateIdToShortGameId<SuperposedGameStateA<Realisation>>(startGameState);
    else if (flavour == "b") shortGameId = gameStateIdToShortGameId<SuperposedGameStateB<Realisation>>(startGameState);
    else if (flavour == "c") shortGameId = gameStateIdToShortGameId<SuperposedGameStateC<Realisation>>(startGameState);
    else if (flavour == "cprime") shortGameId = gameStateIdToShortGameId<SuperposedGameStateCPrime<Realisation>>(startGameState);
    else if (flavour == "d") shortGameId = gameStateIdToShortGameId<SuperposedGameStateD<Realisation>>(startGameState);
    else throw(std::domain_error("Unknown flavour."));

    ShortGame& shortGame = ShortGameDatabase::getInstance().getGame(shortGameId);

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
        else { // The value of the starting position is not a number
            std::cout << shortGame.determineCanonicalForm().determineDisplayString() << std::endl;
        }
    }
    else if (function == "birthday") {
        size_t birthday = shortGame.determineBirthday();
        std::cout << birthday << std::endl;
    }
    else throw(std::domain_error("Unknown function."));
}

int main(int argc, char **argv) {
    if (argc < 4) {
        std::cout << "Usage: qhs <function>[outcome,value,birthday] <position>[hackenbushString_<length>, shortHollyhocks_<nBlueHalves>_<nRedHalves>_<nBlueWholes>_<nRedWholes>, circusTent_<nLegs>, file] <flavour>[classical,a,b,c,cprime,d] <max_width>(optional)" << std::endl;
        return 1;
    }
    std::string function = argv[1];
    std::string position = argv[2];
    std::string flavour = argv[3];
    // If a maximum width is set, use this value
    if (argc > 4) width = std::stoi(argv[4]);

    // Split position string on "_"
    std::stringstream stream(position);
    std::string part;
    std::vector<std::string> parts;
    char delimiter = '_';
    while (getline(stream, part, delimiter)) parts.push_back(part);

    if (parts[0] == "hackenbushString") {
        if (parts.size() != 2) {
            std::cout << "hackenbushString position should be formatted as hackenbushString_<length>" << std::endl;
            return 1;
        }
        size_t length = std::stoi(parts[1]);

        for (const HackenbushStringPosition& start : createHackenbushStringPositions(length)) {
            GameStateId startGameState = GameStateDatabase<HackenbushStringPosition>::getInstance().getOrInsert(start).getId();
            analyse<HackenbushStringPosition>(function, startGameState, flavour);
        }
    }
    else if (parts[0] == "shortHollyhocks") {
        if (parts.size() != 5) {
            std::cout << "halvesWholes positon string should be formatted as shortHollyhocks_<nBlueHalves>_<nRedHalves>_<nBlueWholes>_<nRedWholes>" << std::endl;
            return 1;
        }
        size_t nBlueHalves = std::stoi(parts[1]);
        size_t nRedHalves = std::stoi(parts[2]);
        size_t nBlueWholes = std::stoi(parts[3]);
        size_t nRedWholes = std::stoi(parts[4]);

        const ShortHollyhocksPosition start = createShortHollyhocksPosition(nBlueHalves, nRedHalves, nBlueWholes, nRedWholes);
        #ifdef DEBUG
        start.printHumanReadable();
        #endif
        GameStateId startGameState = GameStateDatabase<ShortHollyhocksPosition>::getInstance().getOrInsert(start).getId();
        analyse<ShortHollyhocksPosition>(function, startGameState, flavour);
    }
    else if (parts[0] == "circusTent") {
        if (parts.size() != 2) {
            std::cout << "circusTent position string should be formatted as circusTent_<nLegs>" << std::endl;
            return 1;
        }
        size_t nLegs = std::stoi(parts[1]);

        // TODO: use more efficient CircusTentPosition representation
        const AdjacencyMatrixPosition start = createCircusTentAdjacencyMatrixPosition(nLegs);
        #ifdef DEBUG
        start.printHumanReadable();
        #endif
        GameStateId startGameState = GameStateDatabase<AdjacencyMatrixPosition>::getInstance().getOrInsert(start).getId();
        analyse<AdjacencyMatrixPosition>(function, startGameState, flavour);
    }
    else if (parts[0] == "file") {
        // TODO: support loading standard format from position file
        std::cout << "Not yet implemented." << std::endl;
    }
    else throw(std::domain_error("Unknown position type."));

    return 0;
}
