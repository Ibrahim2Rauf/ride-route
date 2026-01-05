#ifndef MATCH_H
#define MATCH_H

#include "../Graph/Graph.h"     
class DriverList;              
#define MAX_DRIVERS_INDEX 1000  
#define MAX_MATCH_CAND 256

struct MatchResult {
    int driverID;
    double distance;   
    double totalCost; 
};

class MatchEngine {
private:
    Graph* graph;                
    DriverList* drivers;         
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
    int matchByCheapestFare(int riderID); 
    int matchByNearest(int srcNode); 

    
    int matchRide(int riderID, int srcNode, int dstNode, bool preferCheapest, MatchResult &outResult);
};

#endif
