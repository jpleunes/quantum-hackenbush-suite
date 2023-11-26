#ifndef QUANTUMHACKENBUSH_H
#define QUANTUMHACKENBUSH_H

#include <stdexcept>
#include <string>
#include <vector>

#include "Position.h"

enum class OutcomeClass {
    L,
    R,
    N,
    P,
};

/**
 * An abstract class representing a ruleset of Quantum Hackenbush.
 */
class QuantumHackenbush {
public:
    QuantumHackenbush(Position *position);

    virtual std::vector<QuantumHackenbush*> getBlueOptions() const = 0;
    virtual std::vector<QuantumHackenbush*> getRedOptions() const = 0;
    virtual OutcomeClass determineOutcomeClass() const = 0;

    virtual ~QuantumHackenbush() = default;

protected:
    Position *position;
};

#endif // QUANTUMHACKENBUSH_H
