#ifndef HISTORY_H
#define HISTORY_H

#include <string>
using namespace std;

struct RideHistory {
    int rideID;
    int riderID;
    int driverID;
    int pickupNode;
    int dropNode;
    double fare;
    string status;   // Completed / Cancelled

    RideHistory* next;
};

class HistoryList {
private:
    RideHistory* head;

    int generateRideID();

public:
    HistoryList();
    ~HistoryList();

    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);

    void addHistory(int riderID, int driverID, int pickup, int drop, double fare, string status);

    void viewRiderHistory(int riderID);
    void viewDriverHistory(int driverID);
    void viewAllHistory();  // Admin
};

#endif
