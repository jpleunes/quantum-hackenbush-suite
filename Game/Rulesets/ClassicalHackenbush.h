#ifndef CLASSICALHACKENBUSH_H
#define CLASSICALHACKENBUSH_H

#include "../QuantumHackenbush.h"

class ClassicalHackenbush : public QuantumHackenbush {
public:
    ClassicalHackenbush(const ClassicalPosition *position);
    std::vector<QuantumHackenbush*> getBlueOptions() const override;
    std::vector<QuantumHackenbush*> getRedOptions() const override;
    OutcomeClass determineOutcomeClass() const override;

    ~ClassicalHackenbush() override;
};

#endif // CLASSICALHACKENBUSH_H
