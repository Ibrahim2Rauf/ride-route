#ifndef MATCH_H
#define MATCH_H

#include "../Graph/Graph.h"     
class DriverList;              
#define MAX_DRIVERS_INDEX 1000  // driver IDs expected to be <= 999 (adjust if needed)
#define MAX_MATCH_CAND 256

struct MatchResult {
    int driverID;
    double distance;    // distance in graph units (INF if unknown)
    double totalCost; // farePerKm * distance (or farePerKm if distance unknown)
};

class MatchEngine {
private:
    Graph* graph;                // pointer to graph (not owning)
    DriverList* drivers;         // pointer to driver list (not owning)

    // store driver location externally (driverID -> node). -1 = unknown.
    // We'll make this map reasonably large; if driver IDs exceed,hum constant adjust krskte hain.
    int driverLocation[MAX_DRIVERS_INDEX];

    // internal heap for matching (min-heap of MatchResult by totalCost)
    struct HeapItem {
        int driverID;
        double totalCost;
        int distance;
    }; 
     

    void heapSwap(HeapItem* A, int i, int j);
    void heapPush(HeapItem* A, int &size, HeapItem item);
    HeapItem heapPop(HeapItem* A, int &size);
public: 
    MatchEngine();
    void linkGraph(Graph* g);
    void linkDrivers(DriverList* d);

    // set driver current location (node number)
    void setDriverLocation(int driverID, int node);

    // match functions:
    // match cheapest available driver (uses farePerKm only)
    // returns driverID or -1 if none
    int matchByCheapestFare(int riderID);

    // match nearest driver to 'src' (requires driverLocation set for drivers)
    // returns driverID or -1 if none available
    int matchByNearest(int srcNode);

    // combined match: preferCheapest = true chooses cheapest; otherwise nearest
    int matchRide(int riderID, int srcNode, int dstNode, bool preferCheapest, MatchResult &outResult);
};

#endif
