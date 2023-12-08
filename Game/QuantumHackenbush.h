#ifndef QUANTUM_HACKENBUSH_H
#define QUANTUM_HACKENBUSH_H

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
    QuantumHackenbush(const Position *position);

    virtual std::vector<QuantumHackenbush*> getBlueOptions() const = 0; // TODO: use IEnumerable with yield for better performance
    virtual std::vector<QuantumHackenbush*> getRedOptions() const = 0;
    OutcomeClass determineOutcomeClass() const;

    virtual ~QuantumHackenbush() = default;

protected:
    const Position *position;
};

#endif // QUANTUM_HACKENBUSH_H
