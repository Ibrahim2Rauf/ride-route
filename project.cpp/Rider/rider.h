#ifndef RIDER_H        // header guard start (multiple inclusion se bachata hai)
#define RIDER_H

#include <string>      // string data type ke liye
using namespace std;

// Rider structure
struct Rider {
    int id;            // unique rider ID
    string name;       // rider ka name
    string phone;      // phone number
    string password;   // login password
    double wallet;     // wallet balance
    Rider* next;       // next rider ka pointer (linked list)
};

// RiderList class
class RiderList {
private:
    Rider* head;       // linked list ka starting point

public:
    RiderList();       // constructor
    ~RiderList();      // destructor

    int generateID();  // unique ID generate karta hai
    void loadFromFile(const string& filename); // file se data load
    void saveToFile(const string& filename);   // file mein data save

    void registerRider();                 // naya rider register
    bool loginRider(int &riderID);        // rider login
    void viewRiderProfile(int riderID);   // profile view
    void addWallet(int riderID, double amount); // wallet add
    void printAll();                      // admin ke liye list print

    // Helper to get Rider pointer by ID
    Rider* getRider(int riderID);          // rider pointer return karta hai

    // DELETE
    void deleteRider(int riderID);         // rider delete
};

#endif   // header guard end
