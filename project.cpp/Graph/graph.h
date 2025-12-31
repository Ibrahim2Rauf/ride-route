#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NODES 100       // Graph me maximum nodes ka limit
#define INF 1000000000      // Infinite distance (path nahi hai)

// Edge structure: adjacency list ke liye
struct Edge {
    int to;                 // Edge kis node pe ja rahi hai
    int weight;             // Edge ka weight (distance ya cost)
    Edge* next;             // Next edge pointer (linked list)
};

class Graph {
private:
    Edge* adj[MAX_NODES + 1];   // Adjacency list array (1-based indexing)
    int nodeCount;            
    // Min-heap item structure Dijkstra ke liye
    struct HeapItem {
        int node;              
        int dist;  };           

    // ---------- Min-heap helper functions ----------
    void heapSwap(HeapItem* A, int i, int j);             // Heap me do items swap karna
    void heapPush(HeapItem* heap, int &size, HeapItem item); // Heap me naya item add karna (heapify up)
    HeapItem heapPop(HeapItem* heap, int &size);          // Heap se smallest item nikalna (heapify down)

public:
    Graph();                     
    ~Graph();                    

    void setNodeCount(int n);    // Graph ka size set karna
    int getNodeCount();          // Graph me nodes ka count return karna

    void addEdge(int u, int v, int weight);  // Node u se v tak edge add karna

    // Dijkstra algorithm: shortest path find karna
    int dijkstra(int src, int dest, int distance[], int prev[]);

    int shortestDistance(int src, int dest); // Sirf shortest distance return karna
    int getShortestPath(int src, int dest, int path[], int &pathLen); // Shortest path nodes return karna
};

#endif
