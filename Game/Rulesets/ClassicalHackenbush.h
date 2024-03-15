#ifndef CLASSICAL_HACKENBUSH_H
#define CLASSICAL_HACKENBUSH_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class ClassicalHackenbush : public QuantumHackenbush<Realisation> {
public:
    ClassicalHackenbush(const Superposition<Realisation> superposition);
    bool operator==(const ClassicalHackenbush<Realisation>& other) const;
    std::vector<GameInstanceId> getMoveOptions(Player player) const override;

    ~ClassicalHackenbush() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
ClassicalHackenbush<Realisation>::ClassicalHackenbush(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
bool ClassicalHackenbush<Realisation>::operator==(const ClassicalHackenbush<Realisation>& other) const {
    return this->superposition == other.superposition;
}

template<typename Realisation>
std::vector<GameInstanceId> ClassicalHackenbush<Realisation>::getMoveOptions(Player player) const {
    std::vector<typename Realisation::Piece> pieces = this->superposition.getPieces(player);
    std::vector<GameInstanceId> result;
    for (size_t i = 0; i < pieces.size(); i++) {
        Realisation& position = PositionDatabase<Realisation>::getInstance().getPosition(*this->superposition.getRealisationIds().begin());
        PositionId newPositionId = position.applyMove(pieces[i]);
        GameInstanceId gameInstanceId = GameInstanceDatabase<ClassicalHackenbush<Realisation>>::getInstance().getGameInstanceId(ClassicalHackenbush<Realisation>(Superposition<Realisation>(newPositionId)));
        result.emplace_back(gameInstanceId);
    }
    return result;
};

namespace std {
    template<typename Realisation>
    struct hash<ClassicalHackenbush<Realisation>> {
        size_t operator()(const ClassicalHackenbush<Realisation>& classicalHackenbush) const {
            return std::hash<Superposition<Realisation>>()(classicalHackenbush.getSuperposition());
        }
    };
}

#endif // CLASSICAL_HACKENBUSH_H
