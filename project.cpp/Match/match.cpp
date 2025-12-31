#include "Match.h"
#include "../Graph/Graph.h"
#include "../Driver/Driver.h"
#include <cstring> // memset ke liye
#include <iostream>
using namespace std;

#define INF_DIST 1000000000 
// ------------------- Constructor -------------------
// Initialize MatchEngine: abhi graph aur drivers link nahi hain, driver locations unknown
MatchEngine::MatchEngine() {
    graph = nullptr;   // Graph module abhi link nahi hua
    drivers = nullptr; // Driver module abhi link nahi hua
    for (int i = 0; i < MAX_DRIVERS_INDEX; ++i) driverLocation[i] = -1; // sab driver location unknown
}

// ------------------- Module Linking -------------------
// Ye methods dusre modules se link karte hain
void MatchEngine::linkGraph(Graph* g) { graph = g; }          // Graph module link
void MatchEngine::linkDrivers(DriverList* d) { drivers = d; } // Driver module link

// ------------------- Set driver location -------------------
// Har driver ka current node location set karte hain
void MatchEngine::setDriverLocation(int driverID, int node) {
    if (driverID >= 0 && driverID < MAX_DRIVERS_INDEX)
        driverLocation[driverID] = node;
}

// ------------------- Heap Helpers -------------------
// Heap me swapping
void MatchEngine::heapSwap(HeapItem* A, int i, int j) {
    HeapItem tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

// Heap push: new driver ko heap me add karna (min-heap based on totalCost)
void MatchEngine::heapPush(HeapItem* A, int &size, HeapItem item) {
    int i = ++size;  // new item ko end me add
    A[i] = item;
    while (i > 1) {
        int p = i >> 1; // parent index
        if (A[p].totalCost <= A[i].totalCost) break; // min-heap property maintain
        heapSwap(A, p, i); // swap if parent cost > current
        i = p;
    }
}

// Heap pop: smallest totalCost driver ko remove karna aur heap maintain karna
MatchEngine::HeapItem MatchEngine::heapPop(HeapItem* A, int &size) {
    HeapItem res = A[1]; // smallest element
    A[1] = A[size--];     // last element ko top pe laao
    int i = 1;

    while (1) {
        int l = i << 1;   // left child
        int r = l + 1;    // right child
        int s = i;        // assume smallest = i
        if (l <= size && A[l].totalCost < A[s].totalCost) s = l; // compare left
        if (r <= size && A[r].totalCost < A[s].totalCost) s = r; // compare right
        if (s == i) break; // heap property ok
        heapSwap(A, i, s); // swap with smallest child
        i = s;             // move down
    }
    return res; // return smallest
}

// ------------------- Match by Cheapest Fare -------------------
// Rider ke liye sabse cheap available driver select karna
int MatchEngine::matchByCheapestFare(int riderID) {
    if (drivers == nullptr) return -1; // driver module linked nahi

    int bestDriver = -1;
    double bestFare = 1e18; // initially max

    for (int id = 1; id < MAX_DRIVERS_INDEX; ++id) { // scan all driver IDs
        Driver* d = drivers->getDriver(id);
        if (!d) continue;                  // driver exist nahi karta
        if (d->status != "Available") continue; // sirf available drivers
        if (d->farePerKm < bestFare) {     // sabse cheap fare update
            bestFare = d->farePerKm;
            bestDriver = d->id;
        }
    }
    return bestDriver;
}

// ------------------- Match by Nearest Driver -------------------
// Rider ke source node ke closest available driver select karna
int MatchEngine::matchByNearest(int srcNode) {
    if (drivers == nullptr || graph == nullptr) return -1;

    int bestDriver = -1;
    int bestDist = INF_DIST;

    for (int id = 1; id < MAX_DRIVERS_INDEX; ++id) {
        int loc = driverLocation[id]; // driver current node
        if (loc == -1) continue;     // location unknown
        Driver* d = drivers->getDriver(id);
        if (!d) continue;
        if (d->status != "Available") continue;

        int dist = graph->shortestDistance(loc, srcNode); // driver se pickup
        if (dist < bestDist) {
            bestDist = dist;
            bestDriver = id;
        }
    }

    return bestDriver;
}

// ------------------- Main Ride Matching -------------------
// Rider ke liye driver select karna
int MatchEngine::matchRide(int riderID, int srcNode, int dstNode, bool preferCheapest, MatchResult &outResult) {
    if (drivers == nullptr) return -1;

    // ------------------- Cheapest Preference -------------------
    if (preferCheapest) {
        int chosen = matchByCheapestFare(riderID);
        if (chosen == -1) return -1;
        Driver* d = drivers->getDriver(chosen);
        if (!d) return -1;

        int rideDistance = graph->shortestDistance(srcNode, dstNode); // pickup se drop
        if (rideDistance >= INF_DIST) { // route unreachable
            cout << "Route not reachable!\n";
            return -1;
        }

        double totalCost = rideDistance * d->farePerKm; // calculate fare
        outResult.driverID = chosen;
        outResult.distance = rideDistance;
        outResult.totalCost = totalCost;
        return chosen;
    }

    // ------------------- Nearest Preference -------------------
    if (graph == nullptr) return matchByCheapestFare(riderID); // fallback

    // Heap me available drivers push karna (distance*fare)
    HeapItem heapArr[MAX_MATCH_CAND];
    int heapSize = 0;
    heapArr[0] = HeapItem{0,0.0,0}; // unused index 0

    for (int id = 1; id < MAX_DRIVERS_INDEX; ++id) {
        int loc = driverLocation[id];
        if (loc == -1) continue; // location unknown
        Driver* d = drivers->getDriver(id);
        if (!d) continue;
        if (d->status != "Available") continue;

        int dist = graph->shortestDistance(loc, srcNode);
        if (dist >= INF_DIST) continue; // unreachable
        double cost = d->farePerKm * (double)dist;
        heapPush(heapArr, heapSize, HeapItem{id, cost, dist});
    }

    if (heapSize == 0) { // fallback cheapest
        int f = matchByCheapestFare(riderID);
        if (f == -1) return -1;
        Driver* d = drivers->getDriver(f);
        outResult.driverID = f;
        outResult.distance = -1;
        outResult.totalCost = d->farePerKm;
        return f;
    }

    // Choose the driver with minimum cost
    HeapItem chosen = heapPop(heapArr, heapSize);
    outResult.driverID = chosen.driverID;
    outResult.distance = chosen.distance;
    outResult.totalCost = chosen.totalCost;
    return chosen.driverID;
}
