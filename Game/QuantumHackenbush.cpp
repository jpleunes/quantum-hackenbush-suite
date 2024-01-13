#include <stdexcept>

#include "QuantumHackenbush.h"

QuantumHackenbush::QuantumHackenbush(const Position *position) : position(position) {
}

OutcomeClass QuantumHackenbush::determineOutcomeClass() const {
    auto leftStartOutcome = determineOutcomeClass(Player::LEFT);
    auto rightStartOutcome = determineOutcomeClass(Player::RIGHT);

    // We do not need to check whether both left and right can win when they start, because such 
    // situations are impossible (Hackenbush does not have N-positions).
    if (leftStartOutcome == OutcomeClass::L || leftStartOutcome == OutcomeClass::P) return OutcomeClass::L;
    else if (rightStartOutcome == OutcomeClass::R || rightStartOutcome == OutcomeClass::P) return OutcomeClass::R;
    else return OutcomeClass::P;
}

OutcomeClass QuantumHackenbush::determineOutcomeClass(Player turn) const {
    Generator<QuantumHackenbush*> optionsGen = options(turn);
    switch (turn) {
        case Player::LEFT:
            while (optionsGen) {
                QuantumHackenbush *option = optionsGen();
                OutcomeClass outcome = option->determineOutcomeClass(Player::RIGHT);
                delete option;
                if (outcome == OutcomeClass::L || outcome == OutcomeClass::P) return OutcomeClass::L;
            }
            return OutcomeClass::R;
        case Player::RIGHT:
            while (optionsGen) {
                QuantumHackenbush *option = optionsGen();
                OutcomeClass outcome = option->determineOutcomeClass(Player::LEFT);
                delete option;
                if (outcome == OutcomeClass::R || outcome == OutcomeClass::P) return OutcomeClass::R;
            }
            return OutcomeClass::L;
        default:
            throw(std::domain_error("Unknown player case"));
    }
};
