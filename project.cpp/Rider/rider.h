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

// RiderList class
class RiderList {
private:
    Rider* head;       

public:
    RiderList();       // constructor
    ~RiderList();      // destructor

    int generateID(); 
    void loadFromFile(const string& filename); 
    void saveToFile(const string& filename);   

    void registerRider();                 
    bool loginRider(int &riderID);        
    void viewRiderProfile(int riderID);  
    void addWallet(int riderID, double amount); 
    void printAll();                     

    // Helper to get Rider pointer by ID
    Rider* getRider(int riderID);          

    // DELETE
    void deleteRider(int riderID);         
};

#endif   // header guard end
