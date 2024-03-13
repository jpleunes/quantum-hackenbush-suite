#include <queue>
#include <iostream>

#include "AdjacencyMatrixPosition.h"
#include "PositionDatabase.h"

AdjacencyMatrixPosition::AdjacencyMatrixPosition() : AdjacencyMatrixPosition(0) {
}

AdjacencyMatrixPosition::AdjacencyMatrixPosition(AdjacencyMatrixPosition const& other) : adjacencyMatrix(other.adjacencyMatrix) { // Do NOT copy the cache
}

AdjacencyMatrixPosition::AdjacencyMatrixPosition(size_t nodeCount) {
    adjacencyMatrix.resize(nodeCount);
    // A Hackenbush position is represented as an undirected graph, so we only store the lower triangle of the adjacency matrix.
    for (size_t i = 0; i < nodeCount; i++) {
        adjacencyMatrix[i].resize(i + 1, PieceColour::NONE);
    }
}

bool AdjacencyMatrixPosition::operator==(const AdjacencyMatrixPosition& other) const {
    if (adjacencyMatrix.size() != other.adjacencyMatrix.size()) return false;
    for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
        if (adjacencyMatrix[i].size() != other.adjacencyMatrix[i].size()) return false;
        for (size_t j = 0; j < adjacencyMatrix[i].size(); j++)
            if (adjacencyMatrix[i][j] != other.adjacencyMatrix[i][j]) return false;
    }
        
    return true;
}

void AdjacencyMatrixPosition::addPiece(Edge piece, PieceColour colour) {
    if (piece.from < piece.to) {
        adjacencyMatrix[piece.to][piece.from] = colour;
    }
    else {
        adjacencyMatrix[piece.from][piece.to] = colour;
    }
    // TODO: check if this piece is connected to ground?
}

std::vector<Edge> AdjacencyMatrixPosition::getPieces(Player player) const {
    if (player == Player::LEFT && cache.leftPieces.has_value()) return cache.leftPieces.value();
    if (player == Player::RIGHT && cache.rightPieces.has_value()) return cache.rightPieces.value();

    PieceColour colour;
    switch (player) {
        case Player::LEFT:
            colour = PieceColour::BLUE;
            break;
        case Player::RIGHT:
            colour = PieceColour::RED;
            break;
        default:
            throw(std::domain_error("Unknown player case."));
    }

    std::vector<Edge> pieces;
    for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
        for (size_t j = 0; j < adjacencyMatrix[i].size(); j++) {
            if (adjacencyMatrix[i][j] == colour) {
                pieces.push_back({ i, j });
            }
        }
    }
    if (player == Player::LEFT) cache.leftPieces = pieces;
    if (player == Player::RIGHT) cache.rightPieces = pieces;
    return pieces;
}

PositionId AdjacencyMatrixPosition::applyMove(Edge piece) const {
    if (cache.moveOptions.contains(piece)) return cache.moveOptions[piece];

    AdjacencyMatrixPosition result = AdjacencyMatrixPosition(adjacencyMatrix.size());
    for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
        for (size_t j = 0; j < adjacencyMatrix[i].size(); j++) {
            result.adjacencyMatrix[i][j] = adjacencyMatrix[i][j];
        }
    }

    bool removeSuccessful = false;
    if (piece.from < piece.to) {
        if (adjacencyMatrix[piece.to][piece.from] != PieceColour::NONE) removeSuccessful = true;
        result.adjacencyMatrix[piece.to][piece.from] = PieceColour::NONE;
    }
    else {
        if (adjacencyMatrix[piece.from][piece.to] != PieceColour::NONE) removeSuccessful = true;
        result.adjacencyMatrix[piece.from][piece.to] = PieceColour::NONE;
    }
    cache.moveOptions[piece] = ILLEGAL_POSITION_ID;
    if (!removeSuccessful) return ILLEGAL_POSITION_ID;

    // Remove all pieces which are no longer connected to ground
    result.removeNotConnectedToGround(piece);

    // TODO: remove unreachable nodes by resizing matrix (for this we need node id translations)
    PositionId id = PositionDatabase<AdjacencyMatrixPosition>::getInstance().getPositionId(result);
    cache.moveOptions[piece] = id;
    return id;
}

void AdjacencyMatrixPosition::removeNotConnectedToGround(Edge removedPiece) {
    std::vector<bool> visited(adjacencyMatrix.size(), false);
    std::queue<NodeId> queue;
    queue.push(groundId);
    bool firstVisited = false;
    bool secondVisited = false;
    while (!queue.empty()) {
        NodeId current = queue.front();
        queue.pop();
        visited[current] = true;
        firstVisited |= current == removedPiece.from;
        secondVisited |= current == removedPiece.to;
        if (firstVisited && secondVisited) return;
        for (size_t neighbour = 0; neighbour < adjacencyMatrix.size(); neighbour++) {
            const PieceColour pieceColour = (current < neighbour) ? adjacencyMatrix[neighbour][current] : adjacencyMatrix[current][neighbour];
            if (pieceColour != PieceColour::NONE && !visited[neighbour]) {
                queue.push(neighbour);
            }
        }
    }
    // Once we have all reachable nodes, we remove all pieces connected to unreachable nodes
    for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
        if (visited[i]) continue;
        for (size_t j = 0; j < adjacencyMatrix.size(); j++) {
            if (i < j) {
                adjacencyMatrix[j][i] = PieceColour::NONE;
            }
            else {
                adjacencyMatrix[i][j] = PieceColour::NONE;
            }
        }
    }
}

void AdjacencyMatrixPosition::removeNotConnectedToGround() {
    removeNotConnectedToGround(Edge(-1, -1));
}

void AdjacencyMatrixPosition::printHumanReadable() const {
    std::cout << "===== AdjacencyMatrixPosition =====" << std::endl;
    for (auto row : adjacencyMatrix) {
        for (auto pieceColour : row) {
            if (pieceColour == PieceColour::BLUE) std::cout << "B";
            else if (pieceColour == PieceColour::RED) std::cout << "R";
            else if (pieceColour == PieceColour::NONE) std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "===================================" << std::endl;
}
