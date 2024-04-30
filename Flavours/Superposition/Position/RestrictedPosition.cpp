#include <stdexcept>
#include <iostream>

#include "RestrictedPosition.h"

RestrictedPosition::RestrictedPosition() : entries() {
}

RestrictedPosition::RestrictedPosition(const RestrictedPosition& other) : entries(other.entries) {
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

std::set<RestrictedPiece> RestrictedPosition::getPieces(Player player) const {
    std::set<RestrictedPiece> result;
    for (size_t index = 0; index < entries.size(); index++) {
        HalfOrWhole entry = entries[index];
        if (player == Player::LEFT && (entry == HalfOrWhole::BLUE_HALF || entry == HalfOrWhole::RED_HALF || entry == HalfOrWhole::BLUE_WHOLE))
            result.insert(RestrictedPiece(index, PieceColour::BLUE));
        else if (player == Player::RIGHT && (entry == HalfOrWhole::BLUE_HALF || entry == HalfOrWhole::RED_HALF || entry == HalfOrWhole::RED_WHOLE))
            result.insert(RestrictedPiece(index, PieceColour::RED));
    }
    return result;
}

bool RestrictedPosition::removePiece(RestrictedPiece piece) {
    HalfOrWhole entry = entries[piece.index];
    switch (piece.colour) {
        case PieceColour::BLUE: 
            if (entry == HalfOrWhole::BLUE_HALF || entry == HalfOrWhole::BLUE_WHOLE) {
                entries[piece.index] = HalfOrWhole::NONE;
                break;
            }
            else if (entry == HalfOrWhole::RED_HALF) {
                entries[piece.index] = HalfOrWhole::RED_WHOLE;
                break;
            }

            return false;
       case PieceColour::RED: 
            if (entry == HalfOrWhole::RED_HALF || entry == HalfOrWhole::RED_WHOLE) {
                entries[piece.index] = HalfOrWhole::NONE;
                break;
            }
            else if (entry == HalfOrWhole::BLUE_HALF) {
                entries[piece.index] = HalfOrWhole::BLUE_WHOLE;
                break;
            }

            return false;
        default:
            throw(std::domain_error("Unsupported colour case."));
    }
    return true;
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
