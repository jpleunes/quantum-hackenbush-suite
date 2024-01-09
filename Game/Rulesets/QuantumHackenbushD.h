#ifndef QUANTUM_HACKENBUSHD_H
#define QUANTUM_HACKENBUSHD_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushD : public QuantumHackenbush {
public:
    QuantumHackenbushD(const Position *position);
    std::vector<QuantumHackenbush*> getBlueOptions() const override;
    std::vector<QuantumHackenbush*> getRedOptions() const override;

    ~QuantumHackenbushD() override;
};

#endif // QUANTUM_HACKENBUSHD_H
