#include "ClassicalHackenbush.h"

ClassicalHackenbush::ClassicalHackenbush(const ClassicalPosition* classicalPosition) : QuantumHackenbush(new Position(classicalPosition)) {
};

std::vector<QuantumHackenbush*> ClassicalHackenbush::getBlueOptions() const {
    std::vector<QuantumHackenbush*> blueOptions;
    std::vector<Edge> bluePieces = position->getBluePieces();
    for (size_t i = 0; i < bluePieces.size(); i++) {
        ClassicalPosition* newPosition = position->getRealisation(0).clone();
        newPosition->removePiece(bluePieces[i].first, bluePieces[i].second);
        blueOptions.push_back(new ClassicalHackenbush(newPosition));
    }
    return blueOptions;
};

std::vector<QuantumHackenbush*> ClassicalHackenbush::getRedOptions() const {
    std::vector<QuantumHackenbush*> redOptions;
    std::vector<Edge> redPieces = position->getRedPieces();
    for (size_t i = 0; i < redPieces.size(); i++) {
        ClassicalPosition* newPosition = position->getRealisation(0).clone();
        newPosition->removePiece(redPieces[i].first, redPieces[i].second);
        redOptions.push_back(new ClassicalHackenbush(newPosition));
    }
    return redOptions;
};

OutcomeClass ClassicalHackenbush::determineOutcomeClass() const {
    if (position->empty()) {
        // We move to an empty superposition from a losing position->
        // This means that the empty superposition has outcome class N, 
        // which is the opposite of the outcome class of a losing position (P).
        return OutcomeClass::N;
    }
    auto bluePieces = position->getBluePieces();
    auto redPieces = position->getRedPieces();
    if (bluePieces.empty() && redPieces.empty()) {
        // There are no pieces, so this is a P-position.
        return OutcomeClass::P;
    }
    else if (!bluePieces.empty() && redPieces.empty()) {
        return OutcomeClass::L;
    }
    else if (bluePieces.empty() && !redPieces.empty()) {
        return OutcomeClass::R;
    }

    // We are not in any of the base cases, so we need to use recursion.
    std::vector<QuantumHackenbush*> blueOptions = getBlueOptions();
    std::vector<QuantumHackenbush*> redOptions = getRedOptions();
    std::vector<OutcomeClass> blueOutcomes, redOutcomes;
    for (auto blueOption : blueOptions) {
        blueOutcomes.push_back(blueOption->determineOutcomeClass());
        delete blueOption;
    }
    for (auto redOption : redOptions) {
        redOutcomes.push_back(redOption->determineOutcomeClass());
        delete redOption;
    }


    for (OutcomeClass blueOutcome : blueOutcomes) {
        if (blueOutcome == OutcomeClass::L || blueOutcome == OutcomeClass::P) {
            for (OutcomeClass redOutcome : redOutcomes) {
                if (redOutcome == OutcomeClass::R || redOutcome == OutcomeClass::P) {
                    return OutcomeClass::N;
                }
            }
            return OutcomeClass::L;
        }
    }
    for (OutcomeClass redOutcome : redOutcomes) {
        if (redOutcome == OutcomeClass::R || redOutcome == OutcomeClass::P) {
            return OutcomeClass::R;
        }
    }
    return OutcomeClass::P;
};

ClassicalHackenbush::~ClassicalHackenbush() {
    delete position;
};
