#ifndef GRAPH_H
#define GRAPH_H


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

   
    struct HeapItem {
        int node;
        int dist;
    };

    void heapSwap(HeapItem* A, int i, int j);
    void heapPush(HeapItem* heap, int &size, HeapItem item);
    HeapItem heapPop(HeapItem* heap, int &size);

public:

 
    Graph();
    ~Graph();

  
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
