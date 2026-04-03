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
    
    int driverLocation[MAX_DRIVERS_INDEX];

    
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


    void setDriverLocation(int driverID, int node);
    int matchByCheapestFare(int riderID); 
    int matchByNearest(int srcNode); 

    
    int matchRide(int riderID, int srcNode, int dstNode, bool preferCheapest, MatchResult &outResult);
};

#endif
