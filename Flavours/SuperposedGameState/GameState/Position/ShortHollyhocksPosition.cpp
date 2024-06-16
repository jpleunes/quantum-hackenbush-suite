#include <stdexcept>
#include <iostream>

#include "ShortHollyhocksPosition.h"

ShortHollyhocksPosition::ShortHollyhocksPosition() : shortHollyhocks() {
}

ShortHollyhocksPosition::ShortHollyhocksPosition(const ShortHollyhocksPosition& other) : shortHollyhocks(other.shortHollyhocks) {
}

bool ShortHollyhocksPosition::operator==(const ShortHollyhocksPosition& other) const {
    if (shortHollyhocks.size() != other.shortHollyhocks.size()) return false;
    for (size_t i = 0; i < shortHollyhocks.size(); i++)
        if (shortHollyhocks[i] != other.shortHollyhocks[i]) return false;
    return true;
}

void ShortHollyhocksPosition::addPiece(ShortHollyhocksPiece piece) {
    if (piece.index >= shortHollyhocks.size()) shortHollyhocks.resize(piece.index + 1, ShortHollyhock::NONE);

    ShortHollyhock shortHollyhock = shortHollyhocks[piece.index];
    switch (piece.colour) {
        case PieceColour::BLUE:
            if (shortHollyhock == ShortHollyhock::NONE) {
                shortHollyhocks[piece.index] = ShortHollyhock::BLUE_WHOLE;
                return;
            }
            else if (shortHollyhock == ShortHollyhock::RED_WHOLE) {
                shortHollyhocks[piece.index] = ShortHollyhock::RED_HALF;
                return;
            }
            throw(std::logic_error("Cannot add piece to Short Hollyhocks position at given index."));
        case PieceColour::RED:
            if (shortHollyhock == ShortHollyhock::NONE) {
                shortHollyhocks[piece.index] = ShortHollyhock::RED_WHOLE;
                return;
            }
            else if (shortHollyhock == ShortHollyhock::BLUE_WHOLE) {
                shortHollyhocks[piece.index] = ShortHollyhock::BLUE_HALF;
                return;
            }
            throw(std::logic_error("Cannot add piece to Sort Hollyhocks position at given index."));
        default:
            throw(std::domain_error("Unsupported colour case."));
    }
}

const std::vector<ShortHollyhock>& ShortHollyhocksPosition::getShortHollyhocks() const {
    return shortHollyhocks;
}

std::set<ShortHollyhocksPiece> ShortHollyhocksPosition::getPieces(Player player) const {
    std::set<ShortHollyhocksPiece> result;
    for (size_t index = 0; index < shortHollyhocks.size(); index++) {
        const ShortHollyhock shortHollyhock = shortHollyhocks[index];
        if (player == Player::LEFT && (shortHollyhock == ShortHollyhock::BLUE_HALF || shortHollyhock == ShortHollyhock::RED_HALF || shortHollyhock == ShortHollyhock::BLUE_WHOLE))
            result.insert(ShortHollyhocksPiece(index, PieceColour::BLUE));
        else if (player == Player::RIGHT && (shortHollyhock == ShortHollyhock::BLUE_HALF || shortHollyhock == ShortHollyhock::RED_HALF || shortHollyhock == ShortHollyhock::RED_WHOLE))
            result.insert(ShortHollyhocksPiece(index, PieceColour::RED));
    }
    return result;
}

bool ShortHollyhocksPosition::removePiece(ShortHollyhocksPiece piece) {
    ShortHollyhock shortHollyhock = shortHollyhocks[piece.index];
    switch (piece.colour) {
        case PieceColour::BLUE: 
            if (shortHollyhock == ShortHollyhock::BLUE_HALF || shortHollyhock == ShortHollyhock::BLUE_WHOLE) {
                shortHollyhocks[piece.index] = ShortHollyhock::NONE;
                break;
            }
            else if (shortHollyhock == ShortHollyhock::RED_HALF) {
                shortHollyhocks[piece.index] = ShortHollyhock::RED_WHOLE;
                break;
            }

            return false;
       case PieceColour::RED: 
            if (shortHollyhock == ShortHollyhock::RED_HALF || shortHollyhock == ShortHollyhock::RED_WHOLE) {
                shortHollyhocks[piece.index] = ShortHollyhock::NONE;
                break;
            }
            else if (shortHollyhock == ShortHollyhock::BLUE_HALF) {
                shortHollyhocks[piece.index] = ShortHollyhock::BLUE_WHOLE;
                break;
            }

            return false;
        default:
            throw(std::domain_error("Unsupported colour case."));
    }
    return true;
}

void ShortHollyhocksPosition::printHumanReadable() const {
    std::cout << "===== ShortHollyhocksPosition =====" << std::endl;
    for (ShortHollyhock shortHollyhock : shortHollyhocks) {
        if (shortHollyhock == ShortHollyhock::BLUE_HALF) std::cout << "BH ";
        else if (shortHollyhock == ShortHollyhock::RED_HALF) std::cout << "RH ";
        else if (shortHollyhock == ShortHollyhock::BLUE_WHOLE) std::cout << "BW ";
        else if (shortHollyhock == ShortHollyhock::RED_WHOLE) std::cout << "RW ";
        else std::cout << "NO ";
    }
    std::cout << std::endl;
    std::cout << "==============================" << std::endl;
}
