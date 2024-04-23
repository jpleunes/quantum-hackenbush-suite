#include <stdexcept>
#include <iostream>

#include "RestrictedPosition.h"
#include "PositionDatabase.h"

RestrictedPosition::RestrictedPosition() : entries() {
}

RestrictedPosition::RestrictedPosition(const RestrictedPosition& other) : entries(other.entries) { // Do NOT copy the cache
}

bool RestrictedPosition::operator==(const RestrictedPosition& other) const {
    if (entries.size() != other.entries.size()) return false;
    for (size_t i = 0; i < entries.size(); i++)
        if (entries[i] != other.entries[i]) return false;
    return true;
}

void RestrictedPosition::addPiece(RestrictedPiece piece) {
    if (piece.index >= entries.size()) entries.resize(piece.index + 1, HalfOrWhole::NONE);

    HalfOrWhole entry = entries[piece.index];
    switch (piece.colour) {
        case PieceColour::BLUE:
            if (entry == HalfOrWhole::NONE) {
                entries[piece.index] = HalfOrWhole::BLUE_WHOLE;
                return;
            }
            else if (entry == HalfOrWhole::RED_WHOLE) {
                entries[piece.index] = HalfOrWhole::RED_HALF;
                return;
            }
            throw(std::logic_error("Cannot add piece to restricted position at given index."));
        case PieceColour::RED:
            if (entry == HalfOrWhole::NONE) {
                entries[piece.index] = HalfOrWhole::RED_WHOLE;
                return;
            }
            else if (entry == HalfOrWhole::BLUE_WHOLE) {
                entries[piece.index] = HalfOrWhole::BLUE_HALF;
                return;
            }
            throw(std::logic_error("Cannot add piece to restricted position at given index."));
        default:
            throw(std::domain_error("Unsupported colour case."));
    }
}

std::vector<RestrictedPiece> RestrictedPosition::getPieces(Player player) const {
    if (player == Player::LEFT && cache.leftPieces.has_value()) return cache.leftPieces.value();
    if (player == Player::RIGHT && cache.rightPieces.has_value()) return cache.rightPieces.value();

    std::vector<RestrictedPiece> result;
    for (size_t index = 0; index < entries.size(); index++) {
        HalfOrWhole entry = entries[index];
        if (player == Player::LEFT && (entry == HalfOrWhole::BLUE_HALF || entry == HalfOrWhole::RED_HALF || entry == HalfOrWhole::BLUE_WHOLE))
            result.push_back(RestrictedPiece(index, PieceColour::BLUE));
        else if (player == Player::RIGHT && (entry == HalfOrWhole::BLUE_HALF || entry == HalfOrWhole::RED_HALF || entry == HalfOrWhole::RED_WHOLE))
            result.push_back(RestrictedPiece(index, PieceColour::RED));
    }
    if (player == Player::LEFT) cache.leftPieces = result;
    if (player == Player::RIGHT) cache.rightPieces = result;
    return result;
}

PositionId RestrictedPosition::applyMove(RestrictedPiece piece) const {
    if (cache.moveOptions.contains(piece)) return cache.moveOptions[piece];

    RestrictedPosition result = RestrictedPosition(*this);

    HalfOrWhole entry = entries[piece.index];
    switch (piece.colour) {
        case PieceColour::BLUE: 
            if (entry == HalfOrWhole::BLUE_HALF || entry == HalfOrWhole::BLUE_WHOLE) {
                result.entries[piece.index] = HalfOrWhole::NONE;
                break;
            }
            else if (entry == HalfOrWhole::RED_HALF) {
                result.entries[piece.index] = HalfOrWhole::RED_WHOLE;
                break;
            }
            cache.moveOptions[piece] = ILLEGAL_POSITION_ID;
            return ILLEGAL_POSITION_ID;
       case PieceColour::RED: 
            if (entry == HalfOrWhole::RED_HALF || entry == HalfOrWhole::RED_WHOLE) {
                result.entries[piece.index] = HalfOrWhole::NONE;
                break;
            }
            else if (entry == HalfOrWhole::BLUE_HALF) {
                result.entries[piece.index] = HalfOrWhole::BLUE_WHOLE;
                break;
            }
            cache.moveOptions[piece] = ILLEGAL_POSITION_ID;
            return ILLEGAL_POSITION_ID;
        default:
            throw(std::domain_error("Unsupported colour case."));
    }
    PositionId id = PositionDatabase<RestrictedPosition>::getInstance().getPositionId(result);
    cache.moveOptions[piece] = id;
    return id;
}

void RestrictedPosition::printHumanReadable() const {
    std::cout << "===== RestrictedPosition =====" << std::endl;
    for (HalfOrWhole entry : entries) {
        if (entry == HalfOrWhole::BLUE_HALF) std::cout << "BH ";
        else if (entry == HalfOrWhole::RED_HALF) std::cout << "RH ";
        else if (entry == HalfOrWhole::BLUE_WHOLE) std::cout << "BW ";
        else if (entry == HalfOrWhole::RED_WHOLE) std::cout << "RW ";
        else std::cout << "NO ";
    }
    std::cout << std::endl;
    std::cout << "==============================" << std::endl;
}
