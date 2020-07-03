#pragma once
#include <map>
#include <vector>
#include <array>
#include <unordered_set>
#include <algorithm>

// NOTE: Implementation only allows for node IDs that are integers

class Graph {
    public:
        Graph(std::vector<std::unordered_set<int>>, std::map<std::array<int, 2>, int>);
        Graph(int);
        const int V() const;
        const int E() const;
        bool hasEdge(int x, int y) const;
        const std::unordered_set<int> neighbors(int x) const;
        const std::unordered_set<int> neighbors(int x, int filterVal) const;
        void addEdge (int x, int y);
        void deleteEdge (int x, int y);
        const int getNodeVal(int nodeID) const;
        void setNodeVal(int nodeID, int val);
        const int getEdgeWeight(int x, int y) const;
        void setEdgeWeight(int x, int y, int val);
        
        ~Graph();

    private:
        std::vector<std::unordered_set<int>> vertConnections;
        std::map<std::array<int, 2>, int> edgeWeights;
        int numEdges;
        int numVertices;
        std::vector<int> values;
};