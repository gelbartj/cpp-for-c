#include "graph.hpp"
#include <numeric>
#include <iostream>

using std::cout, std::endl;

Graph::Graph(std::vector<std::unordered_set<int>> vertConnections, std::map<std::array<int, 2>, int> edgeWeights):
vertConnections(vertConnections), edgeWeights(edgeWeights), numVertices(vertConnections.size()) {
    int edgeCount = std::accumulate(vertConnections.begin(), vertConnections.end(), 0, 
        [&](int init, auto &i){ return init + i.size(); });
    numEdges = edgeCount / 2;
}

Graph::Graph(int nodeCount): numEdges(0), numVertices(nodeCount), 
    values(std::vector<int>(nodeCount)), 
    vertConnections(std::vector<std::unordered_set<int>>(nodeCount)) {

}

const int Graph::V() const {
    return numVertices;
}

const int Graph::E() const {
    return numEdges;
}

bool Graph::hasEdge(int x, int y) const {
    for (const int vert : vertConnections[x]) {
        if (vert == y) return true;
    }
    return false;
}

const std::unordered_set<int> Graph::neighbors(int x) const {
    // need to check for out of bounds?
    return vertConnections[x];
}

const std::unordered_set<int> Graph::neighbors(int x, int filterVal) const {
    // need to check for out of bounds?
    // ----- NOT WORKING ----
    std::unordered_set<int> filteredConnections = vertConnections[x];
    // std::remove_if(filteredConnections.begin(), filteredConnections.end(), 
    //    [this, filterVal](int i){ return getNodeVal(i) != filterVal; });
    for (int i : filteredConnections) {
        if (getNodeVal(i) != filterVal) {
            filteredConnections.erase(i);
        }
    }
    return filteredConnections;
}

void Graph::addEdge (int x, int y) {
    vertConnections[x].insert(y);
    vertConnections[y].insert(x);
    ++numEdges;
}

void Graph::deleteEdge (int x, int y) {
    auto& edges = vertConnections[x];
    edges.erase(y);

    edges = vertConnections[y];
    edges.erase(x);
    --numEdges;
}

const int Graph::getEdgeWeight(int x, int y) const {
    const std::array<int, 2> edgePair = { x, y };
    try {
        return edgeWeights.at(edgePair);
    }
    catch (std::out_of_range e) {
        return 0;
    }
}

void Graph::setEdgeWeight(int x, int y, int val) {
    const std::array<int, 2> edgePair = { x, y };
    const std::array<int, 2> reversePair = { y, x };
    edgeWeights[edgePair] = val;
    edgeWeights[reversePair] = val;
}

const int Graph::getNodeVal(int nodeID) const {
    return values[nodeID];
}

void Graph::setNodeVal(int nodeID, int val) {
    values[nodeID] = val;
}

Graph::~Graph() {

}



