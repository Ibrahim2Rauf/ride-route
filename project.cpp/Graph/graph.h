#ifndef GRAPH_H
#define GRAPH_H

/* ================== CONSTANTS ================== */
#define MAX_NODES 100
#define INF 1000000000

/* ================== EDGE STRUCT (ADJACENCY LIST) ================== */
struct Edge {
    int to;
    int weight;
    Edge* next;
};

/* ================== GRAPH CLASS ================== */
class Graph {
private:

    /* ===== ADJACENCY LIST ===== */
    Edge* adj[MAX_NODES + 1];
    int nodeCount;

    /* ===== MIN-HEAP ITEM STRUCT ===== */
    struct HeapItem {
        int node;
        int dist;
    };

    /* ===== MIN-HEAP HELPER FUNCTIONS ===== */
    void heapSwap(HeapItem* A, int i, int j);
    void heapPush(HeapItem* heap, int &size, HeapItem item);
    HeapItem heapPop(HeapItem* heap, int &size);

public:

    /* ===== CONSTRUCTOR / DESTRUCTOR ===== */
    Graph();
    ~Graph();

    /* ===== GRAPH SIZE FUNCTIONS ===== */
    void setNodeCount(int n);
    int getNodeCount();

    /* ===== ADD EDGE ===== */
    void addEdge(int u, int v, int weight);

    /* ===== DIJKSTRA SHORTEST PATH ===== */
    int dijkstra(int src, int dest, int distance[], int prev[]);

    /* ===== SHORTEST DISTANCE ONLY ===== */
    int shortestDistance(int src, int dest);

    /* ===== SHORTEST PATH NODES LIST ===== */
    int getShortestPath(int src, int dest, int path[], int &pathLen);
};

#endif
