#include "hexBoard.hpp"
#include "../dijkstra/shortestPath.hpp"
#include <iostream>
#include <string>

using std::cout, std::endl;

const int HexBoard::nodeIdxFromCoords(int i, int j) const {
    return i * size + j;
}

HexBoard::HexBoard(int size): boardGraph(Graph(size * size + 4)), size(size) {
    topNode = size * size;
    rightNode = size * size + 1;
    bottomNode = size * size + 2;
    leftNode = size * size + 3;

    redMarker = "- ";
    for (int i = 0; i < size * 3; ++i) {
        redMarker += "- ";
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int currNode = nodeIdxFromCoords(i, j);

            if (i - 1 >= 0) boardGraph.addEdge(currNode, nodeIdxFromCoords(i - 1, j));
            if (i + 1 < size) boardGraph.addEdge(currNode, nodeIdxFromCoords(i + 1, j));
            if (j - 1 >= 0) boardGraph.addEdge(currNode, nodeIdxFromCoords(i, j - 1));
            if (j + 1 < size) boardGraph.addEdge(currNode, nodeIdxFromCoords(i, j + 1));
            if (i - 1 >= 0 && j + 1 < size) boardGraph.addEdge(currNode, 
                nodeIdxFromCoords(i - 1, j + 1));
            if (i + 1 < size && j - 1 >= 0) boardGraph.addEdge(currNode, 
                nodeIdxFromCoords(i + 1, j - 1));

            /* 
            if (j - 1 >= 0) boardGraph.addEdge(currNode, nodeIdxFromCoords(i - 1, j - 1));
            if (j + 1 < size) boardGraph.addEdge(currNode, nodeIdxFromCoords(i + 1, j + 1));

            1, 1 connects to 0, 1; 0, 2; 1, 0; 1, 2; 2, 0; 2, 1;
            i - 1, j
            i - 1, j + 1
            i, j - 1
            i, j + 1
            i + 1, j - 1
            i + 1, j
            */
        }
    }

    for (int i = 0; i < size; ++i) {
        boardGraph.addEdge(topNode, i);
    }
    for (int i = size * (size - 1); i < size * size; ++i) {
        boardGraph.addEdge(bottomNode, i);
    }
    for (int i = 0; i < size * (size - 1); i += size) {
        boardGraph.addEdge(leftNode, i);
    }
    for (int i = size - 1; i < size * size; i += size) {
        boardGraph.addEdge(rightNode, i);
    }

    boardGraph.setNodeVal(topNode, static_cast<int>(SpaceStatus::RED));
    boardGraph.setNodeVal(bottomNode, static_cast<int>(SpaceStatus::RED));
    boardGraph.setNodeVal(leftNode, static_cast<int>(SpaceStatus::BLUE));
    boardGraph.setNodeVal(rightNode, static_cast<int>(SpaceStatus::BLUE));
}

SpaceStatus HexBoard::getValue(int i, int j) const {
    return static_cast<SpaceStatus>(boardGraph.getNodeVal(
        nodeIdxFromCoords(i, j)
    ));
}

std::vector<int> HexBoard::longestPath(SpaceStatus status) const {
    return std::vector<int>(1);
}

const SpaceStatus HexBoard::getWinner() const {
    ShortestPath winPath = ShortestPath(boardGraph, topNode, true, static_cast<int>(SpaceStatus::RED));

    /* cout << "Calculating winner for graph..." << endl;
    for (int i = 0; i < boardGraph.V(); ++i) {
        cout << "Node " << i << " has neighbors ";
        for (int j : boardGraph.neighbors(i)) {
            cout << j << ", ";
        }
        cout << "Or filtered blue neighbors ";
        for (int j : boardGraph.neighbors(i, static_cast<int>(SpaceStatus::BLUE))) {
            cout << j << ", ";
        }
        cout << "Or filtered red neighbors ";
        for (int j : boardGraph.neighbors(i, static_cast<int>(SpaceStatus::RED))) {
            cout << j << ", ";
        }
    } */

    if (winPath.pathCost(bottomNode) != DOUBLE_INF) {
        return SpaceStatus::RED;
    }

    winPath = ShortestPath(boardGraph, leftNode, true, static_cast<int>(SpaceStatus::BLUE));

    if (winPath.pathCost(rightNode) != DOUBLE_INF) {
        return SpaceStatus::BLUE;
    }
    
    return SpaceStatus::EMPTY;
}

const int HexBoard::getSize() const {
    return size;
}

void HexBoard::printBoard() const {
    cout << endl;
    cout << "\x1B[1;31m" << redMarker << "\033[0m" << endl << endl;
    for (int i = 0; i < size * size; ++i) {
        if (i % size == 0) {
            // start of new line
            cout << "\x1B[1;34m" << blueMarker << "\033[0m";
            cout << std::string(2 * floor(i / size), ' ');
        }
        
        int rawVal = boardGraph.getNodeVal(i);
        switch (rawVal) {
            case 0:
             cout << ".";
             break;
            case 1:
             cout << "\x1B[1;34m" << "B" << "\033[0m";
             break;
            case 2:
             cout << "\x1B[1;31m" << "R" << "\033[0m";
        }

        if (i == size * size - 1) {
            cout << std::string("  ") << "\x1B[1;34m" << "|" << "\033[0m";
            cout << endl;
            break;
        }

        if ((i + 1) % (size) == 0) {
            // end of line
            cout << std::string(2 * size + 1 - (2 * floor(i / size) + 1), ' ') << "\x1B[1;34m" << "|" << "\033[0m";
            cout << endl << std::string(2 * floor(i / size) + 1 + strlen(blueMarker), ' ') << "\\ ";
            for (int j = 0; j < size - 1; ++j) {
                cout << "/ \\ ";
            }
            cout << endl;
        }
        else cout << " - ";
    }
    cout << endl << "\x1B[1;31m" << redMarker << "\033[0m";
}

void HexBoard::setValue(int i, int j, SpaceStatus val) {
    boardGraph.setNodeVal(nodeIdxFromCoords(i, j), static_cast<int>(val));
}

const bool HexBoard::isLegalMove(int i, int j, SpaceStatus move) const {
    if (getValue(i, j) == SpaceStatus::EMPTY && move != SpaceStatus::EMPTY && i < size && j < size && i >= 0 && j >= 0) {
        return true;
    }
    return false;
}

/*
int main(int argc, char **argv) {
    HexBoard hb = HexBoard(11);
    hb.printBoard();
}
*/