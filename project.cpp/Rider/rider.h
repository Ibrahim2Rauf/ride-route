#ifndef RIDER_H
#define RIDER_H

#include <string>
using namespace std;

struct Rider {
    int id;
    string name;
    string phone;
    string password;
    double wallet;
    Rider* next;
};

class RiderList {
private:
    Rider* head;
public:
    RiderList();
    ~RiderList();

    int generateID();
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);

    void registerRider();
    bool loginRider(int &riderID);
    void viewRiderProfile(int riderID);
    void addWallet(int riderID, double amount);
    void printAll();   

    // Helper to get Rider pointer by ID (optional, useful for menu)
    Rider* getRider(int riderID);

    // DELETE
    void deleteRider(int riderID);
};

#endif
