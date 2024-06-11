#ifndef SHORT_HOLLYHOCKS_POSITION_H
#define SHORT_HOLLYHOCKS_POSITION_H

#include "Position.h"
#include "../../../../Util/HashUtil.h"

enum class HalfOrWhole : char {
    BLUE_HALF,
    RED_HALF,
    BLUE_WHOLE,
    RED_WHOLE,
    NONE
};

class ShortHollyhocksPosition : public Position<ShortHollyhocksPiece> {
public:
    typedef ShortHollyhocksPiece Piece;

    ShortHollyhocksPosition();
    ShortHollyhocksPosition(const ShortHollyhocksPosition& other);
    bool operator==(const ShortHollyhocksPosition& other) const;

    void addPiece(ShortHollyhocksPiece piece);
    bool removePiece(ShortHollyhocksPiece piece) override;
    std::set<ShortHollyhocksPiece> getPieces(Player player) const override;
    void printHumanReadable() const override;

    ~ShortHollyhocksPosition() override = default;

    std::vector<HalfOrWhole> entries;
};

namespace std {
    template<>
    struct hash<ShortHollyhocksPosition> {
        size_t operator()(const ShortHollyhocksPosition& position) const {
            std::vector<uint32_t> intVec;
            for (HalfOrWhole x : position.entries) intVec.push_back((uint32_t) x);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }
    };
}

#endif // SHORT_HOLLYHOCKS_POSITION_H
