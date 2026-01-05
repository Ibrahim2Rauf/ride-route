#include "Vehicle.h"         // Vehicle aur VehicleList ke structures
#include <iostream>          
#include <fstream>           
#include <sstream>           
#include <iomanip>           

using namespace std;

// Constructor
VehicleList::VehicleList() {
    head = nullptr;           
}

// Destructor
VehicleList::~VehicleList() {
    Vehicle* temp;
    while (head) {           
        temp = head;
        head = head->next;
        delete temp;          // memory free
    }
}

// Generate unique vehicle ID
int VehicleList::generateVehicleID() {
    int maxID = 0;
    Vehicle* temp = head;
    while (temp) {            // poori list traverse
        if (temp->vehicleID > maxID)
            maxID = temp->vehicleID; 
        temp = temp->next;
    }
    return maxID + 1;         // next unique ID
}

// Load vehicles from file
void VehicleList::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin) return;         

    string line;
    while (getline(fin, line)) { // har line read
        stringstream ss(line);
        Vehicle* v = new Vehicle;

        getline(ss, line, ','); v->vehicleID = stoi(line); 
        getline(ss, line, ','); v->driverID = stoi(line);  
        getline(ss, v->brand, ',');                        
        getline(ss, v->model, ',');                        
        getline(ss, v->color, ',');                        
        getline(ss, v->plateNumber, ',');             
        getline(ss, v->type, ',');                    
        getline(ss, line, ','); v->rating = stod(line);  

        v->next = head;         // front insertion
        head = v;
    }
     fin.close();
}

// Save vehicles to file
void VehicleList::saveToFile(const string& filename) {
    ofstream fout(filename);
     Vehicle* temp = head;
    while (temp) {
        fout << temp->vehicleID << ","//save horha hai vehicle id in file 
             << temp->driverID << ","
             << temp->brand << ","
             << temp->model << ","
             << temp->color << ","
             << temp->plateNumber << ","
             << temp->type << ","
             << fixed << setprecision(1) << temp->rating
             << "\n";
        temp = temp->next;
    }
     fout.close();
}

// Add new vehicle
void VehicleList::addVehicle(int driverID) {
    Vehicle* v = new Vehicle;
    v->vehicleID = generateVehicleID(); // unique vehicleID
    v->driverID = driverID;

    cout << "Enter Brand: "; cin.ignore(); getline(cin, v->brand); 
    cout << "Enter Model: "; getline(cin, v->model);
    cout << "Enter Color: "; getline(cin, v->color);
    cout << "Enter Plate Number: "; getline(cin, v->plateNumber);
    cout << "Vehicle Type (Bike/Car/Rickshaw): "; getline(cin, v->type);

    v->rating = 5.0;          // default rating
    v->next = head;            // front insertion
    head = v;

    saveToFile("Vehicle/vehicles.txt"); // save data
    cout << "Vehicle registered successfully.\n";
}

// Update existing vehicle
void VehicleList::updateVehicle(int driverID) {
    Vehicle* v = getVehicleByDriver(driverID);
    if (!v) { cout << "No vehicle found for this driver.\n"; return; }

    cout << "Update Brand: "; cin.ignore(); getline(cin, v->brand);
    cout << "Update Model: "; getline(cin, v->model);
    cout << "Update Color: "; getline(cin, v->color);
    cout << "Update Plate Number: "; getline(cin, v->plateNumber);
    cout << "Update Type: "; getline(cin, v->type);

    saveToFile("Vehicle/vehicles.txt");
    cout << "Vehicle updated.\n";
}

// Get vehicle by driverID
Vehicle* VehicleList::getVehicleByDriver(int driverID) {
    Vehicle* temp = head;
    while (temp) {
        if (temp->driverID == driverID)
            return temp;        // match mil gaya
        temp = temp->next;
    }
    return nullptr;             // nahi mila
}

// Display vehicle info for driver
void VehicleList::displayVehicle(int driverID) {
    Vehicle* v = getVehicleByDriver(driverID);
    if (!v) { cout << "Vehicle not found.\n"; return; }

    cout << "----- Vehicle Info -----\n";
    cout << "Brand: " << v->brand << "\n";
    cout << "Model: " << v->model << "\n";
    cout << "Color: " << v->color << "\n";
    cout << "Plate: " << v->plateNumber << "\n";
    cout << "Type: " << v->type << "\n";
    cout << "Rating: " << v->rating << "\n";
}

// View all vehicles
void VehicleList::viewAllVehicles() {
    Vehicle* temp = head;
    cout << "------ All Registered Vehicles ------\n";
     while (temp) {
        cout << "[" << temp->vehicleID << "] Driver " << temp->driverID 
             << " | " << temp->brand << " " << temp->model
             << " | Plate: " << temp->plateNumber << "\n";
        temp = temp->next;
    }
}

// DELETE vehicle by driverID
void VehicleList::deleteVehicle(int driverID) {
    Vehicle* cur = head;
    Vehicle* prev = nullptr;

    while (cur) {
        if (cur->driverID == driverID) {
            if (prev == nullptr) head = cur->next;
            else prev->next = cur->next;

            delete cur;               // memory free
            saveToFile("Vehicle/vehicles.txt"); // update file
            cout << "Vehicle deleted.\n";
            return;
        }
        prev = cur;
        cur = cur->next;
    }
    // agar nahi mila, kuch nahi karna
}
