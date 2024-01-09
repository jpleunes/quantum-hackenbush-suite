#ifndef QUANTUM_HACKENBUSHA_H
#define QUANTUM_HACKENBUSHA_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushA : public QuantumHackenbush {
public:
    QuantumHackenbushA(const Position *position);
    std::vector<QuantumHackenbush*> getOptions(Player player) const override;

    ~QuantumHackenbushA() override;
};

#endif // QUANTUM_HACKENBUSHA_H
