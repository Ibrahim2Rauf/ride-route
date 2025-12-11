#include "Match.h"
#include "../Graph/Graph.h"
#include "../Driver/Driver.h"
#include <cstring> // memset
#include <iostream>
using namespace std;

#define INF_DIST 1000000000

// NOTE: DriverList is your Driver module; we use its methods to find drivers in memory.
// We assume DriverList has getDriver(driverID) and iteration via getDriver by scanning (we'll call getDriver on known ids).
// Since DriverList does not provide a direct iterator, we'll scan driver IDs from 1..MAX_DRIVERS_INDEX and call getDriver.
// This is simple and matches your project size expectations.

MatchEngine::MatchEngine() {
    graph = nullptr;
    drivers = nullptr;
    for (int i = 0; i < MAX_DRIVERS_INDEX; ++i) driverLocation[i] = -1;
}

// link modules
void MatchEngine::linkGraph(Graph* g) { graph = g; }
void MatchEngine::linkDrivers(DriverList* d) { drivers = d; }

// driver location
void MatchEngine::setDriverLocation(int driverID, int node) {
    if (driverID >= 0 && driverID < MAX_DRIVERS_INDEX) driverLocation[driverID] = node;
}

// heap helpers
void MatchEngine::heapSwap(HeapItem* A, int i, int j) {
    HeapItem tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}
void MatchEngine::heapPush(HeapItem* A, int &size, HeapItem item) {
    int i = ++size;
    A[i] = item;
    while (i > 1) {
        int p = i >> 1;
        if (A[p].totalCost <= A[i].totalCost) break;
        heapSwap(A, p, i);
        i = p;
    }
}
MatchEngine::HeapItem MatchEngine::heapPop(HeapItem* A, int &size) {
    HeapItem res = A[1];
    A[1] = A[size--];
    int i = 1;
    while (1) {
        int l = i << 1;
        int r = l + 1;
        int s = i;
        if (l <= size && A[l].totalCost < A[s].totalCost) s = l;
        if (r <= size && A[r].totalCost < A[s].totalCost) s = r;
        if (s == i) break;
        heapSwap(A, i, s);
        i = s;
    }
    return res;
}

// match by cheapest farePerKm (ignores distance if driver location unknown)
int MatchEngine::matchByCheapestFare(int riderID) {
    if (drivers == nullptr) return -1;

    int bestDriver = -1;
    double bestFare = 1e18;

    // scan driver id range
    for (int id = 1; id < MAX_DRIVERS_INDEX; ++id) {
        Driver* d = drivers->getDriver(id);
        if (!d) continue;
        if (d->status != "Available") continue;
        if (d->farePerKm < bestFare) {
            bestFare = d->farePerKm;
            bestDriver = d->id;
        }
    }
    return bestDriver;
}

// match by nearest driver to source node (requires driverLocation set)
int MatchEngine::matchByNearest(int srcNode) {
    if (drivers == nullptr || graph == nullptr) return -1;

    int bestDriver = -1;
    int bestDist = INF_DIST;

    for (int id = 1; id < MAX_DRIVERS_INDEX; ++id) {
        int loc = driverLocation[id];
        if (loc == -1) continue;
        Driver* d = drivers->getDriver(id);
        if (!d) continue;
        if (d->status != "Available") continue;

        int dist = graph->shortestDistance(loc, srcNode);
        if (dist < bestDist) {
            bestDist = dist;
            bestDriver = id;
        }
    }

    return bestDriver;
}

// main match function: returns chosen driverID or -1; outResult filled if found
int MatchEngine::matchRide(int riderID, int srcNode, int dstNode, bool preferCheapest, MatchResult &outResult) {
    if (drivers == nullptr) return -1;

    // if preferCheapest: find cheapest available driver, compute distance from their location to src (if known)
    if (preferCheapest) {
        int chosen = matchByCheapestFare(riderID);
        if (chosen == -1) return -1;
        Driver* d = drivers->getDriver(chosen);
        if (!d) return -1;

        int dist = INF_DIST;
        if (chosen >=0 && chosen < MAX_DRIVERS_INDEX && driverLocation[chosen] != -1 && graph != nullptr) {
            dist = graph->shortestDistance(driverLocation[chosen], srcNode);
        } else {
            dist = 0; // unknown distance
        }
        double totalCost = 0.0;
        if (dist >= INF_DIST) totalCost = d->farePerKm; else totalCost = d->farePerKm * (double)dist;

        outResult.driverID = chosen;
        outResult.distance = (dist>=INF_DIST? -1: dist);
        outResult.totalCost = totalCost;
        return chosen;
    }

    // otherwise prefer nearest: build heap of available drivers by cost = distance*fare
    // If driver location unknown, skip them for nearest algorithm.
    if (graph == nullptr) {
        // fallback to cheapest if graph not linked
        return matchByCheapestFare(riderID);
    }

    // Build heap
    HeapItem heapArr[MAX_MATCH_CAND];
    int heapSize = 0;
    heapArr[0] = HeapItem{0,0.0,0}; // unused index 0

    for (int id = 1; id < MAX_DRIVERS_INDEX; ++id) {
        int loc = driverLocation[id];
        if (loc == -1) continue;
        Driver* d = drivers->getDriver(id);
        if (!d) continue;
        if (d->status != "Available") continue;

        int dist = graph->shortestDistance(loc, srcNode);
        if (dist >= INF_DIST) continue; // unreachable

        double cost = d->farePerKm * (double)dist;
        heapPush(heapArr, heapSize, HeapItem{id, cost, dist});
    }

    if (heapSize == 0) {
        // fallback to cheapest
        int f = matchByCheapestFare(riderID);
        if (f == -1) return -1;
        Driver* d = drivers->getDriver(f);
        outResult.driverID = f;
        outResult.distance = -1;
        outResult.totalCost = d->farePerKm;
        return f;
    }

    HeapItem chosen = heapPop(heapArr, heapSize);
    outResult.driverID = chosen.driverID;
    outResult.distance = chosen.distance;
    outResult.totalCost = chosen.totalCost;
    return chosen.driverID;
}
