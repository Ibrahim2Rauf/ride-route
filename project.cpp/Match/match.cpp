#include "Match.h"
#include "../Graph/Graph.h"
#include "../Driver/Driver.h"
#include <cstring>
#include <iostream>
using namespace std;

#define INF_DIST 1000000000 


MatchEngine::MatchEngine() {
    graph = nullptr;
    drivers = nullptr;
    for (int i = 0; i < MAX_DRIVERS_INDEX; ++i) driverLocation[i] = -1;
}


void MatchEngine::linkGraph(Graph* g) { graph = g; }
void MatchEngine::linkDrivers(DriverList* d) { drivers = d; }

void MatchEngine::setDriverLocation(int driverID, int node) {
    if (driverID >= 0 && driverID < MAX_DRIVERS_INDEX)
        driverLocation[driverID] = node;
}


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


int MatchEngine::matchByCheapestFare(int riderID) {
    if (drivers == nullptr) return -1;

    int bestDriver = -1;
    double bestFare = 1e18;

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


int MatchEngine::matchRide(int riderID, int srcNode, int dstNode, bool preferCheapest, MatchResult &outResult) {
    if (drivers == nullptr) return -1;

    if (preferCheapest) {
        int chosen = matchByCheapestFare(riderID);
        if (chosen == -1) return -1;
        Driver* d = drivers->getDriver(chosen);
        if (!d) return -1;

        int rideDistance = graph->shortestDistance(srcNode, dstNode);
        if (rideDistance >= INF_DIST) {
            cout << "Route not reachable!\n";
            return -1;
        }

        double totalCost = rideDistance * d->farePerKm;
        outResult.driverID = chosen;
        outResult.distance = rideDistance;
        outResult.totalCost = totalCost;
        return chosen;
    }


    if (graph == nullptr) return matchByCheapestFare(riderID);

    HeapItem heapArr[MAX_MATCH_CAND];
    int heapSize = 0;
    heapArr[0] = HeapItem{0,0.0,0};

    for (int id = 1; id < MAX_DRIVERS_INDEX; ++id) {
        int loc = driverLocation[id];
        if (loc == -1) continue;
        Driver* d = drivers->getDriver(id);
        if (!d) continue;
        if (d->status != "Available") continue;

        int dist = graph->shortestDistance(loc, srcNode);
        if (dist >= INF_DIST) continue;
        double cost = d->farePerKm * (double)dist;
        heapPush(heapArr, heapSize, HeapItem{id, cost, dist});
    }

    if (heapSize == 0) {
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
