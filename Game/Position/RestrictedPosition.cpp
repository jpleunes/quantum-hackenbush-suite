#include <stdexcept>
#include <iostream>

#include "RestrictedPosition.h"

RestrictedPosition::RestrictedPosition() : entries() {
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
    std::vector<RestrictedPiece> result;
    for (size_t index = 0; index < entries.size(); index++) {
        HalfOrWhole entry = entries[index];
        if (player == Player::LEFT && (entry == HalfOrWhole::BLUE_HALF || entry == HalfOrWhole::RED_HALF || entry == HalfOrWhole::BLUE_WHOLE))
            result.push_back(RestrictedPiece(index, PieceColour::BLUE));
        else if (player == Player::RIGHT && (entry == HalfOrWhole::BLUE_HALF || entry == HalfOrWhole::RED_HALF || entry == HalfOrWhole::RED_WHOLE))
            result.push_back(RestrictedPiece(index, PieceColour::RED));
    }
    return result;
}

Position<RestrictedPiece>* RestrictedPosition::applyMove(RestrictedPiece piece) const {
    RestrictedPosition* result = new RestrictedPosition(*this);

    HalfOrWhole entry = entries[piece.index];
    switch (piece.colour) {
        case PieceColour::BLUE: 
            if (entry == HalfOrWhole::BLUE_HALF || entry == HalfOrWhole::BLUE_WHOLE) {
                result->entries[piece.index] = HalfOrWhole::NONE;
                return result;
            }
            else if (entry == HalfOrWhole::RED_HALF) {
                result->entries[piece.index] = HalfOrWhole::RED_WHOLE;
                return result;
            }
            delete result;
            return nullptr;
       case PieceColour::RED: 
            if (entry == HalfOrWhole::RED_HALF || entry == HalfOrWhole::RED_WHOLE) {
                result->entries[piece.index] = HalfOrWhole::NONE;
                return result;
            }
            else if (entry == HalfOrWhole::BLUE_HALF) {
                result->entries[piece.index] = HalfOrWhole::BLUE_WHOLE;
                return result;
            }
            delete result;
            return nullptr;
        default:
            throw(std::domain_error("Unsupported colour case."));
    }
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
