#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
using namespace std;

struct Vehicle {
    int vehicleID;      // Unique ID
    int driverID;       // Which driver owns this vehicle
    string brand;       
    string model;       
    string color;       
    string plateNumber; 
    string type;        // Bike / Car / Rickshaw
    double rating;      
    Vehicle* next;
};

class VehicleList {
private:
    Vehicle* head;

public:
    VehicleList();
    ~VehicleList();

    int generateVehicleID();
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);

    void addVehicle(int driverID);
    void updateVehicle(int driverID);
    Vehicle* getVehicleByDriver(int driverID);
    void displayVehicle(int driverID);

    void viewAllVehicles(); // admin use

    // DELETE
    void deleteVehicle(int driverID);
};

#endif
