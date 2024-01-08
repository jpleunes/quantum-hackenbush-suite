#ifndef CLASSICAL_HACKENBUSH_H
#define CLASSICAL_HACKENBUSH_H

#include "../QuantumHackenbush.h"

class ClassicalHackenbush : public QuantumHackenbush {
public:
    ClassicalHackenbush(const Position *position);
    std::vector<QuantumHackenbush*> getBlueOptions() const override;
    std::vector<QuantumHackenbush*> getRedOptions() const override;

    ~ClassicalHackenbush() override;
};

#endif // CLASSICAL_HACKENBUSH_H
