#ifndef QUANTUM_HACKENBUSHD_H
#define QUANTUM_HACKENBUSHD_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushD : public QuantumHackenbush {
public:
    QuantumHackenbushD(const Position *position);
    std::vector<QuantumHackenbush*> getOptions(Player player) const override;

    ~QuantumHackenbushD() override;
};

#endif // QUANTUM_HACKENBUSHD_H
