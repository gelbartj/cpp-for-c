#include "shortestPath.hpp"
#include <limits>
#include <stdexcept>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

using std::cout, std::endl;

ShortestPath::ShortestPath(Graph gArg, int startNodeID, bool withValue, int filterValue): g(gArg), currentNode(startNodeID) {
    
    for (int i = 0; i < g.V(); ++i) {
        if (i == startNodeID) {
            unvisited.push(i, 0);
            continue;
        }
        if (!withValue || (withValue && g.getNodeVal(i) == filterValue)) {
            // cout << "=====" << endl;
            // cout << "Adding node " << i << " to unvisited list" << endl;
            // cout << "Node has value " << g.getNodeVal(i) << " compared to requested filter " << filterValue << endl;
            unvisited.push(i, DOUBLE_INF);
        }
    }
    if (withValue) {
     processNodes(withValue, filterValue);
    }
    else {
        processNodes();
    }
}

void ShortestPath::processNodes(bool withValue, int filterValue) {
    if (unvisited.size() == 0) {
        return;
    }
    const PQNode minNode = unvisited.pop();
    visited[minNode.nodeID] = valAndParent { .nodeVal = minNode.nodeVal, .parentID = minNode.parentID };
    currentNode = minNode.nodeID;
    const int startDistance = minNode.nodeVal;
    // cout << "StartDistance: " << startDistance << endl;
    if (withValue) {
        for (int neighborID : g.neighbors(currentNode, filterValue)) {
        unvisited.updateMinIfPresent(neighborID, 
            startDistance + g.getEdgeWeight(currentNode, neighborID), currentNode);
        }
    }   
    else {
        for (int neighborID : g.neighbors(currentNode)) {
        unvisited.updateMinIfPresent(neighborID, 
            startDistance + g.getEdgeWeight(currentNode, neighborID), currentNode);
        }
    }    
    if (unvisited.top().nodeVal != DOUBLE_INF) {
        processNodes();
    }
}

const std::vector<int> ShortestPath::path(int endNode) const {
    std::vector<int> fullPath;
    int currNode = endNode;

    try {
        valAndParent visitedNode = visited.at(currNode);
    }
    catch (std::out_of_range e) {
        // cout << "ERROR: Desired end node is not reachable from provided start node " << endl;
        return fullPath;
    }

    fullPath.push_back(currNode);

    while (visited.at(currNode).parentID != -1) {
        currNode = visited.at(currNode).parentID;
        fullPath.push_back(currNode);
    }

    return fullPath;
}

const int ShortestPath::pathCost(int endNode) const {
    try {
        return visited.at(endNode).nodeVal;
    }
    catch (std::out_of_range e) {
        // cout << "ERROR: Cannot calculate path cost to unreachable end node" << endl;
        return DOUBLE_INF;
    }
}

Graph createRandomGraph(double edgeDensity, int distMin, int distMax) {
    int nodeCount = 50;

    std::vector<std::unordered_set<int>> vertConnections(nodeCount);
    std::map<std::array<int, 2>, int> edgeWeights;
    
    std::random_device dev;
    std::mt19937 rng(dev());

    for (int i = 0; i < nodeCount; ++i) {
        std::unordered_set<int> nodeConnections = vertConnections[i];
        
        for (int j = 0; j < nodeCount; ++j) {
            if (i == j) continue;

            std::uniform_real_distribution<double> randomProb(0.0, 1.0);
            double randomProbability = randomProb(rng);

            if (randomProbability <= edgeDensity) {
                nodeConnections.insert(j);
                vertConnections[j].insert(i);
                std::uniform_int_distribution<std::mt19937::result_type> randomEdgeWeight(distMin, distMax);
                const std::array<int, 2> edgePair = { i, j };
                const std::array<int, 2> reversePair = { j, i };
                edgeWeights[edgePair] = edgeWeights[reversePair] = randomEdgeWeight(rng);
            }
        }
        vertConnections[i] = nodeConnections;
    }
    return Graph(vertConnections, edgeWeights);
}

static std::string imploded(const std::vector<int> &path, const char* const delim) {
    std::ostringstream imploded;
    std::copy(path.rbegin(), path.rend(),
        std::ostream_iterator<int>(imploded, delim));
    return imploded.str().substr(0, imploded.str().size() - strlen(delim));
}

static std::string makeWeightsString(const std::vector<int> &path, const Graph &g) {
    std::ostringstream weightsStream;
    int totalWeight = 0;
    for (auto it = path.rbegin(); it != (path.rend() - 1) ; ++it) {
        const int currEdgeWeight = g.getEdgeWeight(*it, *(it + 1));
        weightsStream << currEdgeWeight;
        if (it != (path.rend() - 2)) weightsStream << ", ";
        totalWeight += currEdgeWeight;
    }
    return weightsStream.str();
}

/*
int main(int argc, char **argv) {
    cout << "Creating graph 1" << endl;
    const Graph g1 = createRandomGraph(0.2, 1, 10);
    cout << "Creating graph 2" << endl;
    const Graph g2 = createRandomGraph(0.5, 1, 10);

    cout << "Creating shortestPath 1" << endl;
    const ShortestPath g1Path = ShortestPath(g1, 0);
    cout << "Creating shortestPath 2" << endl;
    const ShortestPath g2Path = ShortestPath(g2, 0);
    cout << "Created both successfully" << endl;

    double g1AverageCost = 0.0;
    double g2AverageCost = 0.0;

    int g1PathCount = 0;
    int g2PathCount = 0;

    cout << "Calculating averages" << endl;
    for (int i = 1; i < g1.V(); ++i) {
        // cout << "For i = " << i << endl;
        if (g1Path.pathCost(i) != DOUBLE_INF) {
            // cout << "Adding g1 cost " << g1Path.pathCost(i) << " to average" << endl;
            g1AverageCost += g1Path.pathCost(i);
            ++g1PathCount;

            /* UNCOMMENT FOR VERBOSE MODE
            auto path = g1Path.path(i);
            std::string stringPath = imploded(path, " -> ");
            std::string stringWeights = makeWeightsString(path, g1);

            cout << "===================" << endl;
            cout << "Path from 0 to " << i << " is: " << stringPath << endl;
            cout << "Edge weights are: " << stringWeights << endl;
            // cout << "Total weight from iterator: " << totalWeight << endl;
            // cout << "Total weight from visited: " << g1Path.pathCost(i) << endl;
            */ /*
        }
        if (g2Path.pathCost(i) != DOUBLE_INF) {
            g2AverageCost += g2Path.pathCost(i);
            ++g2PathCount;

            /*
            auto path = g2Path.path(i);
            std::string stringPath = imploded(path, "->");
            std::string printedWeights = makeWeightsString(path, g2);

            cout << "*===================" << endl;
            cout << "Path from 0 to " << i << " is: " << stringPath << endl;
            cout << "Edge weights are: " << printedWeights << endl;
            */ /*
            
        }
    }

    g1AverageCost /= g1PathCount;
    g2AverageCost /= g2PathCount;

    cout << std::fixed; cout.precision(2);
    cout << "g1 has " << g1.E() << " edges (" << (static_cast<double>(g1.E()) / (g1.V() * (g1.V() - 1))) * 100.0 << "\% density). average cost: " << g1AverageCost << endl;
    cout << "g2 has " << g2.E() << " edges (" << (static_cast<double>(g2.E()) / (g2.V() * (g2.V() - 1))) * 100.0 << "\% density). average cost: " << g2AverageCost << endl;
}
*/


