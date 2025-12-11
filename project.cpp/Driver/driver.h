#ifndef DRIVER_H
#define DRIVER_H

#include <string>
using namespace std;

#include "../Vehicle/Vehicle.h"   // link vehicle module

struct Driver {
    int id;
    string name;
    string phone;
    string password;
    string cnic;
    int age;
    double rating;
    string status;      
    double farePerKm;
    double wallet;
    string vehicleNumber;
    Driver* next;
};

class DriverList {
private:
    static const int TABLE_SIZE = 10;
    Driver* table[TABLE_SIZE];

    // vehicle module pointer
    VehicleList* vehicles;

    int hashFunc(int id);

public:
    DriverList();
    ~DriverList();

    // link vehicle module
    void linkVehicleModule(VehicleList* v);
    

    int generateID();
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);

    void registerDriver();
    bool loginDriver(int &driverID);
    void viewDriverProfile(int driverID);
    void updateFare(int driverID, double newFare);
    void acceptRide(int driverID);
    
    // DELETE
    void deleteDriver(int driverID);
    Driver* getDriver(int driverID);
    Driver* findByPhone(const string& phone);
};

#endif
