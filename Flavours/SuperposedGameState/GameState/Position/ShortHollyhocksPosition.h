#ifndef SHORT_HOLLYHOCKS_POSITION_H
#define SHORT_HOLLYHOCKS_POSITION_H

#include "Position.h"
#include "../../../../Util/HashUtil.h"

enum class ShortHollyhock : char {
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
    const std::vector<ShortHollyhock>& getShortHollyhocks() const;
    std::set<ShortHollyhocksPiece> getPieces(Player player) const override;
    void printHumanReadable() const override;

    ~ShortHollyhocksPosition() override = default;

    std::vector<ShortHollyhock> shortHollyhocks;
};

namespace std {
    template<>
    struct hash<ShortHollyhocksPosition> {
        size_t operator()(const ShortHollyhocksPosition& position) const {
            std::vector<uint32_t> intVec;
            for (ShortHollyhock x : position.shortHollyhocks) intVec.push_back((uint32_t) x);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }
    };
}

#endif // SHORT_HOLLYHOCKS_POSITION_H
