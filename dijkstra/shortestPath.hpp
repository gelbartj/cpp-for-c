#pragma once
#include <vector>
#include <map>
#include "graph.hpp"
#include "priorityqueue.hpp"

struct valAndParent {
    int nodeVal;
    int parentID;
};

const int DOUBLE_INF = std::numeric_limits<double>::infinity();


class ShortestPath {
    public:
        ShortestPath(Graph gArg, int startNodeID, bool withValue = false, int filterValue = 0);
        const std::vector<int> path(int endNode) const;
        const int pathCost(int endNode) const;

    private:
        PriorityQueue unvisited;
        std::map<const int, valAndParent> visited;
        Graph g;
        int currentNode;
        void processNodes(bool withValue = false, int filterValue = 0);
};