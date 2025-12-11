#ifndef GRAPH_H
#define GRAPH_H

// Simple graph with adjacency list (no STL).
// Nodes are 1..MAX_NODES (you can change MAX_NODES as needed)

#define MAX_NODES 100
#define INF 1000000000

struct Edge {
    int to;
    int weight;
    Edge* next;
};

class Graph {
private:
    Edge* adj[MAX_NODES + 1];
    int nodeCount;

    // Min-heap internals for Dijkstra (arrays)
    struct HeapItem {
        int node;
        int dist;
    };

    // heap functions (used internally)
    void heapSwap(HeapItem* A, int i, int j);
    void heapPush(HeapItem* heap, int &size, HeapItem item);
    HeapItem heapPop(HeapItem* heap, int &size);

public:
    Graph();
    ~Graph();

    // set how many nodes you're using (1..n)
    void setNodeCount(int n);
    int getNodeCount();

    // add directed edge u -> v. If undirected, call twice.
    void addEdge(int u, int v, int weight);

    // Dijkstra: fills distance[] and prev[] arrays passed by caller.
    // distance and prev must be arrays of size at least MAX_NODES+1.
    // Returns 1 if destination reachable, 0 otherwise.
    int dijkstra(int src, int dest, int distance[], int prev[]);

    // Convenience: compute shortest distance only (returns INF if unreachable)
    int shortestDistance(int src, int dest);

    // Convenience: get path as array: returns length of path stored in path[] (path[0..len-1])
    // Caller must pass path array (int path[MAX_NODES]) and will receive node sequence from src -> dest.
    int getShortestPath(int src, int dest, int path[], int &pathLen);
};

#endif
