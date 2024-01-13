#ifndef QUANTUM_HACKENBUSH_C_PRIME_H
#define QUANTUM_HACKENBUSH_C_PRIME_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushCPrime : public QuantumHackenbush {
public:
    QuantumHackenbushCPrime(const Position *position);
    Generator<QuantumHackenbush*> options(Player player) const override;

    ~QuantumHackenbushCPrime() override;
};

#endif // QUANTUM_HACKENBUSH_C_PRIME_H
