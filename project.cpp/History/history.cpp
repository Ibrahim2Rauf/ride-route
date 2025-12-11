#include "History.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

HistoryList::HistoryList() {
    head = nullptr;
}

HistoryList::~HistoryList() {
    RideHistory* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

int HistoryList::generateRideID() {
    int maxID = 0;
    RideHistory* temp = head;
    while (temp) {
        if (temp->rideID > maxID)
            maxID = temp->rideID;
        temp = temp->next;
    }
    return maxID + 1;
}

void HistoryList::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin) return;

    string line;
    while (getline(fin, line)) {
        if (line == "") continue;

        stringstream ss(line);

        RideHistory* h = new RideHistory;

        getline(ss, line, ','); h->rideID = stoi(line);
        getline(ss, line, ','); h->riderID = stoi(line);
        getline(ss, line, ','); h->driverID = stoi(line);
        getline(ss, line, ','); h->pickupNode = stoi(line);
        getline(ss, line, ','); h->dropNode = stoi(line);
        getline(ss, line, ','); h->fare = stod(line);
        getline(ss, h->status, ',');

        h->next = head;
        head = h;
    }

    fin.close();
}

void HistoryList::saveToFile(const string& filename) {
    ofstream fout(filename);

    RideHistory* temp = head;
    while (temp) {
        fout << temp->rideID << ","
             << temp->riderID << ","
             << temp->driverID << ","
             << temp->pickupNode << ","
             << temp->dropNode << ","
             << fixed << setprecision(2) << temp->fare << ","
             << temp->status << "\n";

        temp = temp->next;
    }

    fout.close();
}

void HistoryList::addHistory(int riderID, int driverID,
                             int pickup, int drop,
                             double fare, string status) {
    RideHistory* h = new RideHistory;

    h->rideID = generateRideID();
    h->riderID = riderID;
    h->driverID = driverID;
    h->pickupNode = pickup;
    h->dropNode = drop;
    h->fare = fare;
    h->status = status;

    h->next = head;
    head = h;

    saveToFile("History/history.txt");

    cout << "Ride history saved.\n";
}

void HistoryList::viewRiderHistory(int riderID) {
    RideHistory* temp = head;
    bool found = false;

    cout << "\n===== Rider Ride History =====\n";

    while (temp) {
        if (temp->riderID == riderID) {
            found = true;
            cout << "Ride ID: " << temp->rideID << "\n";
            cout << "Driver ID: " << temp->driverID << "\n";
            cout << "From Node: " << temp->pickupNode << "\n";
            cout << "To Node: " << temp->dropNode << "\n";
            cout << "Fare: " << temp->fare << "\n";
            cout << "Status: " << temp->status << "\n";
            cout << "--------------------------\n";
        }
        temp = temp->next;
    }

    if (!found)
        cout << "No ride history found.\n";
}

void HistoryList::viewDriverHistory(int driverID) {
    RideHistory* temp = head;
    bool found = false;

    cout << "\n===== Driver Ride History =====\n";

    while (temp) {
        if (temp->driverID == driverID) {
            found = true;
            cout << "Ride ID: " << temp->rideID << "\n";
            cout << "Rider ID: " << temp->riderID << "\n";
            cout << "From Node: " << temp->pickupNode << "\n";
            cout << "To Node: " << temp->dropNode << "\n";
            cout << "Fare: " << temp->fare << "\n";
            cout << "Status: " << temp->status << "\n";
            cout << "--------------------------\n";
        }
        temp = temp->next;
    }

    if (!found)
        cout << "No ride history found.\n";
}

void HistoryList::viewAllHistory() {
    RideHistory* temp = head;

    cout << "\n========== ALL RIDE HISTORY (ADMIN) ==========\n";

    while (temp) {
        cout << "Ride ID: " << temp->rideID << "\n";
        cout << "Rider ID: " << temp->riderID << "\n";
        cout << "Driver ID: " << temp->driverID << "\n";
        cout << "Pickup: " << temp->pickupNode << "\n";
        cout << "Drop: " << temp->dropNode << "\n";
        cout << "Fare: " << temp->fare << "\n";
        cout << "Status: " << temp->status << "\n";
        cout << "----------------------------------\n";

        temp = temp->next;
    }
}
