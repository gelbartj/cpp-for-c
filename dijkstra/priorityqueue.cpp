#include <algorithm>
#include <cmath>
#include <iostream>
#include "priorityqueue.hpp"

using std::cout;
using std::endl;

PQNode::PQNode(int nodeID, int nodeVal, int parentID): nodeID(nodeID), nodeVal(nodeVal), parentID(parentID) {

}

PQNode::PQNode(int nodeID, int nodeVal): nodeID(nodeID), nodeVal(nodeVal), parentID(-1) {

}

PQNode::~PQNode() {

}

PriorityQueue::PriorityQueue() {

}

PQNode PriorityQueue::pop() {
    PQNode min = top();
    minHeap[0] = minHeap.back();
    minHeap.pop_back();
    heapifyChildren(0);
    return min;
}

void PriorityQueue::chgValue(int nodeID, int newNodeVal) {
    if (newNodeVal < top().nodeVal) {
        cout << "WARNING: Attempted to change value on node ID not in heap" << endl;
        return;
    }

    auto it = getNode(nodeID);

    if (it != minHeap.end()) {
        it->nodeVal = newNodeVal;
        int itIdx = it - minHeap.begin();
        heapifyChildren(itIdx);
        if (minHeap[itIdx].nodeID == nodeID) { 
            // there has been no change after heapifyChildren
            heapifyParent(itIdx);
        }
    }
    else {
        cout << "WARNING: Attempted to change value on node ID not in heap" << endl;
    }
}

const int PriorityQueue::getValue(int nodeID) const {
    auto it = getNode(nodeID);

    if (it != minHeap.end()) {
        return it->nodeVal;
    }
    else {
        cout << "WARNING: Attempted to get value of node ID not in heap" << endl;
        return -1;
    }
}

const bool PriorityQueue::contains(int nodeID, int nodeVal) const {
    if (nodeVal < top().nodeVal) {
        return false;
    }

    return getNode(nodeID) != minHeap.end();
}

std::vector<PQNode>::iterator PriorityQueue::getNode(int nodeID) {
    return std::find_if(minHeap.begin(), minHeap.end(), 
        [&nodeID](const PQNode &obj) { 
        return obj.nodeID == nodeID;
    });
}

std::vector<PQNode>::const_iterator PriorityQueue::getNode(int nodeID) const {
    // horrible violation of DRY but allows for const-correct function calls
    return std::find_if(minHeap.begin(), minHeap.end(), 
        [&nodeID](const PQNode &obj) { 
        return obj.nodeID == nodeID;
    });
}

const bool PriorityQueue::contains(int nodeID) const {
    return getNode(nodeID) != minHeap.end();
}

void PriorityQueue::push(int nodeID, int nodeVal) {
    minHeap.push_back(PQNode(nodeID, nodeVal));
    heapifyParent(minHeap.size() - 1);
}

PQNode PriorityQueue::top() const {
    return minHeap.front();
}

const int PriorityQueue::size() const {
    return minHeap.size();
}

void PriorityQueue::updateMinIfPresent(int nodeID, int newNodeVal, int parentID) {
    auto it = getNode(nodeID);
    if (it == minHeap.end()) return; 

    if (newNodeVal < it->nodeVal) {
        // cout << "Updating node " << nodeID << " from " << it->nodeVal << " to " << newNodeVal << endl;
        it->nodeVal = newNodeVal;
        it->parentID = parentID;
        int itIdx = it - minHeap.begin();
        heapifyChildren(itIdx);
        if (minHeap[itIdx].nodeID == nodeID) { 
            // there has been no change after heapifyChildren
            heapifyParent(itIdx);
        }
    }
}

PriorityQueue::~PriorityQueue() {

}
    
void PriorityQueue::heapifyChildren(int nodeIdx) {
    int left = 2 * nodeIdx + 1;
    int right = 2 * nodeIdx + 2;
    int minIdx = nodeIdx;
    
    if (left < minHeap.size() && minHeap[left].nodeVal < minHeap[minIdx].nodeVal) {
        minIdx = left;
    }

    if (right < minHeap.size() && minHeap[right].nodeVal < minHeap[minIdx].nodeVal) {
        minIdx = right;
    }
    
    if (minIdx != nodeIdx) {
        PQNode temp = minHeap[nodeIdx];
        minHeap[nodeIdx] = minHeap[minIdx];
        minHeap[minIdx] = temp;

        heapifyChildren(minIdx);
    }
}

void PriorityQueue::heapifyParent(int nodeIdx) {
    int parent = floor((nodeIdx - 1) / 2);
    
    if (parent >= 0 && minHeap[nodeIdx].nodeVal < minHeap[parent].nodeVal) {
        PQNode temp = minHeap[nodeIdx];
        minHeap[nodeIdx] = minHeap[parent];
        minHeap[parent] = temp;

        heapifyParent(parent);
    }
}

/* UNIT TEST

int main(int argc, char** argv) {
    PriorityQueue pq = PriorityQueue();

    pq.push(0, 1);
    pq.push(1, 10);
    pq.push(3, 3);
    pq.push(2, 5);
    pq.push(4, 20);

    cout << "Size is " << pq.size() << endl;
    cout << "Contains 5? " << pq.contains(2, 5) << endl;
    cout << "Top is " << pq.top().nodeVal << endl;

    int startSize = pq.size();
    for (int i = 0; i < startSize; ++i) {
        int min = pq.pop().nodeVal;
        cout << "Min is " << min << endl;
    }

    return 0;
}
*/
