#ifndef QUANTUM_HACKENBUSH_C_PRIME_H
#define QUANTUM_HACKENBUSH_C_PRIME_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushCPrime : public QuantumHackenbush {
public:
    QuantumHackenbushCPrime(const Position *position);
    std::vector<QuantumHackenbush*> getOptions(Player player) const override;

    ~QuantumHackenbushCPrime() override;
};

#endif // QUANTUM_HACKENBUSH_C_PRIME_H
