#include "Graph.h"
#include <cstring> // memset ke liye
#include <iostream>
using namespace std;

// Constructor: graph banate waqt adjacency list initialize karna
Graph::Graph() {
    nodeCount = MAX_NODES;           // initially max nodes ka size
    for (int i = 0; i <= MAX_NODES; ++i) 
        adj[i] = nullptr;            // har node ka adjacency list null se start
}

// Destructor: memory leak na ho, dynamically allocated edges delete karna
Graph::~Graph() {
    for (int i = 0; i <= MAX_NODES; ++i) {
        Edge* e = adj[i];
        while (e) {                  // har node ki linked list traverse
            Edge* t = e;
            e = e->next;             // next edge pe move
            delete t;                // purani edge delete
        }
        adj[i] = nullptr;            // node ka pointer null
    }
}

// Set total nodes (graph ka size)
void Graph::setNodeCount(int n) {
    if (n < 1) n = 1;                // minimum 1 node
    if (n > MAX_NODES) n = MAX_NODES; // max limit
    nodeCount = n;
}

// Get total nodes
int Graph::getNodeCount() { 
    return nodeCount; 
}

// Add edge from u to v with weight
void Graph::addEdge(int u, int v, int weight) {
    if (u < 1 || u > nodeCount || v < 1 || v > nodeCount) return; // invalid node check
    Edge* e = new Edge;               // new edge dynamically create
    e->to = v;                        // destination node
    e->weight = weight;               // edge weight
    e->next = adj[u];                 // front insertion in linked list
    adj[u] = e;                       // head update
}

/* ------------------ Min-heap helper functions ------------------ */

// Swap two heap items
void Graph::heapSwap(HeapItem* A, int i, int j) {
    HeapItem tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

// Insert item into min-heap (heapify-up)
void Graph::heapPush(HeapItem* heap, int &size, HeapItem item) {
    int i = ++size;                   // heap size increase
    heap[i] = item;                   // add item at last
    while (i > 1) {                   // bubble up
        int p = i >> 1;               // parent index (i/2)
        if (heap[p].dist <= heap[i].dist) break; // heap property check
        heapSwap(heap, p, i);         // swap parent and child
        i = p;                        // move up
    }
}

// Remove smallest item from min-heap (heapify-down)
Graph::HeapItem Graph::heapPop(HeapItem* heap, int &size) {
    HeapItem res = heap[1];           // smallest item at root
    heap[1] = heap[size--];           // replace root with last
    int i = 1;
    while (1) {                       // bubble down
        int l = i << 1;               // left child
        int r = l + 1;                // right child
        int smallest = i;
        if (l <= size && heap[l].dist < heap[smallest].dist) smallest = l;
        if (r <= size && heap[r].dist < heap[smallest].dist) smallest = r;
        if (smallest == i) break;     // heap property satisfied
        heapSwap(heap, i, smallest);  // swap parent with smallest child
        i = smallest;
    }
    return res;                       // return smallest item
}

/* ------------------ Dijkstra Algorithm ------------------ */

// Find shortest path from src to dest
int Graph::dijkstra(int src, int dest, int distance[], int prev[]) {
    // Initialize distances and previous nodes
    for (int i = 0; i <= nodeCount; ++i) {
        distance[i] = INF;            // initially infinite distance
        prev[i] = -1;                 // no previous node
    }
    if (src < 1 || src > nodeCount) return 0; // invalid source node
    distance[src] = 0;                // source distance = 0

    // Heap for Dijkstra: 1-based array
    HeapItem* heap = new HeapItem[nodeCount * 2 + 5];
    int heapSize = 0;
    heapPush(heap, heapSize, HeapItem{src, 0}); // push source node

    bool visited[MAX_NODES + 1];      // track visited nodes
    for (int i = 0; i <= nodeCount; ++i) visited[i] = false;

    while (heapSize > 0) {
        HeapItem it = heapPop(heap, heapSize); // node with min distance
        int u = it.node;
        int distU = it.dist;
        if (visited[u]) continue;      // skip if already processed
        visited[u] = true;
        if (u == dest) break;          // destination reached

        Edge* e = adj[u];
        while (e) {                    // traverse adjacency list
            int v = e->to;
            int w = e->weight;
            if (!visited[v] && distance[v] > distU + w) {
                distance[v] = distU + w;   // relax edge
                prev[v] = u;               // store path
                heapPush(heap, heapSize, HeapItem{v, distance[v]}); // push updated node
            }
            e = e->next;
        }
    }

    delete[] heap;                    // free heap memory
    return (distance[dest] < INF) ? 1 : 0; // return 1 if path exists
}

// Get only shortest distance
int Graph::shortestDistance(int src, int dest) {
    int dist[MAX_NODES + 1];
    int prev[MAX_NODES + 1];
    int ok = dijkstra(src, dest, dist, prev);
    if (!ok) return INF;              // path not found
    return dist[dest];                // return distance
}

// Get shortest path nodes
int Graph::getShortestPath(int src, int dest, int path[], int &pathLen) {
    int dist[MAX_NODES + 1];
    int prev[MAX_NODES + 1];
    int ok = dijkstra(src, dest, dist, prev);
    if (!ok) { pathLen = 0; return 0; } // path not found

    // Reconstruct path from dest to src
    int tmp[MAX_NODES + 1];
    int tlen = 0;
    int cur = dest;
    while (cur != -1) {
        tmp[tlen++] = cur;            // store node
        if (cur == src) break;        // reached source
        cur = prev[cur];              // move to previous
    }

    // Reverse path into path[]
    pathLen = 0;
    for (int i = tlen - 1; i >= 0; --i) 
        path[pathLen++] = tmp[i];

    return 1;                        // path found
}
