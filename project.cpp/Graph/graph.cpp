#include "Graph.h"
#include <cstring>
#include <iostream>
using namespace std;


Graph::Graph() {
    nodeCount = MAX_NODES;
    for (int i = 0; i <= MAX_NODES; ++i)
        adj[i] = nullptr;
}


Graph::~Graph() {
    for (int i = 0; i <= MAX_NODES; ++i) {
        Edge* e = adj[i];
        while (e) {
            Edge* t = e;
            e = e->next;
            delete t;
        }
        adj[i] = nullptr;
    }
}

/* ================== NODE COUNT SETTINGS ================== */
void Graph::setNodeCount(int n) {
    if (n < 1) n = 1;
    if (n > MAX_NODES) n = MAX_NODES;
    nodeCount = n;
}

int Graph::getNodeCount() {
    return nodeCount;
}


void Graph::addEdge(int u, int v, int weight) {
    if (u < 1 || u > nodeCount || v < 1 || v > nodeCount) return;
    Edge* e = new Edge;
    e->to = v;
    e->weight = weight;
    e->next = adj[u];
    adj[u] = e;
}

/* ================== MIN HEAP — SWAP ================== */
void Graph::heapSwap(HeapItem* A, int i, int j) {
    HeapItem tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

/* ================== MIN HEAP — PUSH ================== */
void Graph::heapPush(HeapItem* heap, int &size, HeapItem item) {
    int i = ++size;
    heap[i] = item;
    while (i > 1) {
        int p = i >> 1;
        if (heap[p].dist <= heap[i].dist) break;
        heapSwap(heap, p, i);
        i = p;
    }
}

/* ================== MIN HEAP — POP ================== */
Graph::HeapItem Graph::heapPop(HeapItem* heap, int &size) {
    HeapItem res = heap[1];
    heap[1] = heap[size--];
    int i = 1;
    while (1) {
        int l = i << 1;
        int r = l + 1;
        int smallest = i;
        if (l <= size && heap[l].dist < heap[smallest].dist) smallest = l;
        if (r <= size && heap[r].dist < heap[smallest].dist) smallest = r;
        if (smallest == i) break;
        heapSwap(heap, i, smallest);
        i = smallest;
    }
    return res;
}

/* ================== DIJKSTRA ALGORITHM ================== */
int Graph::dijkstra(int src, int dest, int distance[], int prev[]) {

    for (int i = 0; i <= nodeCount; ++i) {
        distance[i] = INF;
        prev[i] = -1;
    }

    if (src < 1 || src > nodeCount) return 0;
    distance[src] = 0;

    HeapItem* heap = new HeapItem[nodeCount * 2 + 5];
    int heapSize = 0;
    heapPush(heap, heapSize, HeapItem{src, 0});

    bool visited[MAX_NODES + 1];
    for (int i = 0; i <= nodeCount; ++i) visited[i] = false;

    while (heapSize > 0) {
        HeapItem it = heapPop(heap, heapSize);
        int u = it.node;
        int distU = it.dist;

        if (visited[u]) continue;
        visited[u] = true;

        if (u == dest) break;

        Edge* e = adj[u];
        while (e) {
            int v = e->to;
            int w = e->weight;

            if (!visited[v] && distance[v] > distU + w) {
                distance[v] = distU + w;
                prev[v] = u;
                heapPush(heap, heapSize, HeapItem{v, distance[v]});
            }
            e = e->next;
        }
    }

    delete[] heap;
    return (distance[dest] < INF) ? 1 : 0;
}

/* ================== SHORTEST DISTANCE ONLY ================== */
int Graph::shortestDistance(int src, int dest) {
    int dist[MAX_NODES + 1];
    int prev[MAX_NODES + 1];
    int ok = dijkstra(src, dest, dist, prev);
    if (!ok) return INF;
    return dist[dest];
}

/* ================== SHORTEST PATH NODES ================== */
int Graph::getShortestPath(int src, int dest, int path[], int &pathLen) {
    int dist[MAX_NODES + 1];
    int prev[MAX_NODES + 1];

    int ok = dijkstra(src, dest, dist, prev);
    if (!ok) { pathLen = 0; return 0; }

    int tmp[MAX_NODES + 1];
    int tlen = 0;

    int cur = dest;
    while (cur != -1) {
        tmp[tlen++] = cur;
        if (cur == src) break;
        cur = prev[cur];
    }

    pathLen = 0;
    for (int i = tlen - 1; i >= 0; --i)
        path[pathLen++] = tmp[i];

    return 1;
}
