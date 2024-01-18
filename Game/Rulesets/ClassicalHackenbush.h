#ifndef CLASSICAL_HACKENBUSH_H
#define CLASSICAL_HACKENBUSH_H

#include "../QuantumHackenbush.h"

class ClassicalHackenbush : public QuantumHackenbush {
public:
    ClassicalHackenbush(const Superposition *superposition);
    Generator<QuantumHackenbush*> options(Player player) const override;

    ~ClassicalHackenbush() override;
};

#endif // CLASSICAL_HACKENBUSH_H
