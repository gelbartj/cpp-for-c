#pragma once
#include "../dijkstra/graph.hpp"
#include <string>
#include <string_view>

enum class SpaceStatus: int { EMPTY = 0, BLUE, RED };

class HexBoard {
    public:
        HexBoard(int size);
        SpaceStatus getValue(int i, int j) const;
        void setValue(int i, int j, SpaceStatus);
        const bool isLegalMove(int i, int j, SpaceStatus) const;
        const SpaceStatus getWinner() const;
        void printBoard() const;
        std::vector<int> longestPath(SpaceStatus) const;
        const int getSize() const;

    private:
        Graph boardGraph;
        int size;
        int topNode;
        int bottomNode;
        int leftNode;
        int rightNode;
        const int nodeIdxFromCoords(int i, int j) const;
        static constexpr char blueMarker[] = "|  ";
        std::string redMarker;
};
