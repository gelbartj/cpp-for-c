#include <vector>

class PQNode {
    public:
        PQNode(int, int, int);
        PQNode(int, int);
        int nodeID;
        int nodeVal;
        int parentID;
        ~PQNode();
};

class PriorityQueue {
    public:
        PriorityQueue();
        void chgValue(int nodeID, int newNodeVal);
        const int getValue(int nodeID) const;
        PQNode pop();
        const bool contains(int nodeID, int nodeVal) const;
        const bool contains(int nodeID) const;
        void updateMinIfPresent(int nodeID, int newNodeVal, int);
        void push(int nodeID, int nodeVal);
        PQNode top() const;
        const int size() const;
        ~PriorityQueue();
    
    private:
        std::vector<PQNode> minHeap;
        std::vector<PQNode>::iterator getNode(int nodeID);
        std::vector<PQNode>::const_iterator getNode(int nodeID) const;
        void heapifyChildren(int);
        void heapifyParent(int);

};

