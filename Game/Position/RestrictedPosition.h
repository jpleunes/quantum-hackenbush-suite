#ifndef RESTRICTED_POSITION_H
#define RESTRICTED_POSITION_H

#include "Position.h"

enum class HalfOrWhole : char {
    BLUE_HALF,
    RED_HALF,
    BLUE_WHOLE,
    RED_WHOLE,
    NONE
};

class RestrictedPosition : public Position<RestrictedPiece> {
public:
    RestrictedPosition();

    void addPiece(RestrictedPiece piece);
    std::vector<RestrictedPiece> getPieces(Player player) const override;
    Position* applyMove(RestrictedPiece piece) const override;
    void printHumanReadable() const override;

    ~RestrictedPosition() override = default;

private:
    std::vector<HalfOrWhole> entries;
};

#endif // RESTRICTED_POSITION_H
