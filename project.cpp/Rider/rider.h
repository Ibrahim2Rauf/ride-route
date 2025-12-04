#ifndef RIDER_H
#define RIDER_H

#include <string>
using namespace std;

// Rider structure
struct Rider {
    int id;
    string name;
    string phone;
    string password;
    double wallet;
    Rider* next;
};

// Linked List of Riders
class RiderList {
private:
    Rider* head;
    int generateID(); // generate unique ID
public:
    RiderList();
    ~RiderList();

    void loadFromFile(const string& filename); // load riders from file
    void saveToFile(const string& filename);   // save riders to file

    void registerRider();       // registration
    bool loginRider(int &riderID);  // login, returns true if successful, sets riderID
    void viewRiderProfile(int riderID); // show rider info
    void addWallet(int riderID, double amount); // add money to wallet
};

#endif
