#include <iostream>

#include "HackenbushStringPosition.h"

HackenbushStringPosition::HackenbushStringPosition() : hackenbushString() {
}

HackenbushStringPosition::HackenbushStringPosition(const HackenbushStringPosition& other) : hackenbushString(other.hackenbushString) {
}

bool HackenbushStringPosition::operator==(const HackenbushStringPosition& other) const {
    return hackenbushString == other.hackenbushString;
}

void HackenbushStringPosition::addPiece(PieceColour colour) {
    hackenbushString.push_back(colour);
}

std::set<HackenbushStringPiece> HackenbushStringPosition::getPieces(Player player) const {
    std::set<HackenbushStringPiece> result;
    for (HackenbushStringPiece piece = 0; piece < hackenbushString.size(); piece++) {
        PieceColour colour = hackenbushString[piece];
        if ((player == Player::LEFT && colour == PieceColour::BLUE) || (player == Player::RIGHT && colour == PieceColour::RED))
            result.insert(piece);
    }
    return result;
}

bool HackenbushStringPosition::removePiece(HackenbushStringPiece piece) {
    if (hackenbushString[piece] == PieceColour::NONE) return false;
    for (HackenbushStringPiece index = piece; index < hackenbushString.size(); index++)
        hackenbushString[index] = PieceColour::NONE;
    return true;
}

void HackenbushStringPosition::printHumanReadable() const {
    std::cout << "==== HackenbushStringPosition ====" << std::endl;
    for (PieceColour colour : hackenbushString) {
        if (colour == PieceColour::BLUE) std::cout << "B";
        else if (colour == PieceColour::RED) std::cout << "R";
        else std::cout << " ";
    }
    std::cout << std::endl;
    std::cout << "==================================" << std::endl;
}
