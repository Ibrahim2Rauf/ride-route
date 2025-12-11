#include "Driver.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

const int DriverList::TABLE_SIZE;

// Constructor
DriverList::DriverList() {
    for (int i = 0; i < TABLE_SIZE; ++i) table[i] = nullptr;
    vehicles = nullptr;   // Added
}

// Link vehicle module
void DriverList::linkVehicleModule(VehicleList* v) {
    vehicles = v;
}

// Destructor
DriverList::~DriverList() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Driver* cur = table[i];
        while (cur) {
            Driver* t = cur;
            cur = cur->next;
            delete t;
        }
        table[i] = nullptr;
    }
}

// Simple hash function (by id)
int DriverList::hashFunc(int id) {
    return id % TABLE_SIZE;
}

// Generate unique ID (scan entire table for max id)
int DriverList::generateID() {
    int maxID = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Driver* cur = table[i];
        while (cur) {
            if (cur->id > maxID) maxID = cur->id;
            cur = cur->next;
        }
    }
    return maxID + 1;
}

// Load drivers from file (CSV per line)
void DriverList::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin) return;

    string line;
    while (getline(fin, line)) {
        if (line.size() == 0) continue;
        stringstream ss(line);
        Driver* d = new Driver;
        string token;

        // id
        getline(ss, token, ','); d->id = stoi(token);
        // name
        getline(ss, d->name, ',');
        // phone
        getline(ss, d->phone, ',');
        // password
        getline(ss, d->password, ',');
        // cnic
        getline(ss, d->cnic, ',');
        // age
        getline(ss, token, ','); d->age = stoi(token);
        // rating
        getline(ss, token, ','); d->rating = stod(token);
        // status
        getline(ss, d->status, ',');
        // farePerKm
        getline(ss, token, ','); d->farePerKm = stod(token);
        // wallet
        getline(ss, token, ','); d->wallet = stod(token);
        // vehicleNumber (last field, may contain no comma)
        getline(ss, d->vehicleNumber);

        // insert into hash table (front insert)
        int idx = hashFunc(d->id);
        d->next = table[idx];
        table[idx] = d;
    }
    fin.close();
}

// Save drivers to file (CSV)
void DriverList::saveToFile(const string& filename) {
    ofstream fout(filename);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Driver* cur = table[i];
        while (cur) {
            fout << cur->id << ","
                 << cur->name << ","
                 << cur->phone << ","
                 << cur->password << ","
                 << cur->cnic << ","
                 << cur->age << ","
                 << fixed << setprecision(2) << cur->rating << ","
                 << cur->status << ","
                 << fixed << setprecision(2) << cur->farePerKm << ","
                 << fixed << setprecision(2) << cur->wallet << ","
                 << cur->vehicleNumber << "\n";
            cur = cur->next;
        }
    }
    fout.close();
}

// Register Driver
void DriverList::registerDriver() {
    Driver* d = new Driver;
    d->id = generateID();
    cout << "Enter Name: "; cin.ignore(); getline(cin, d->name);
    cout << "Enter Phone: "; cin >> d->phone;
    cout << "Enter Password: "; cin >> d->password;
    cout << "Enter CNIC: "; cin >> d->cnic;
    cout << "Enter Age: "; cin >> d->age;
    d->rating = 4.5;
    d->status = "Available";
    cout << "Enter Fare per KM: "; cin >> d->farePerKm;
    d->wallet = 500.0;

    // ⭐ ADD VEHICLE AUTOMATICALLY
    if (vehicles != nullptr) {
        vehicles->addVehicle(d->id);
    }

    d->vehicleNumber = "AUTO";  // Not used now but kept for file compatibility

    int idx = hashFunc(d->id);
    d->next = table[idx];
    table[idx] = d;

    saveToFile("Driver/drivers.txt");
    cout << "Driver registered successfully! Driver ID: " << d->id << "\n";
}

// Find driver by phone
Driver* DriverList::findByPhone(const string& phone) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Driver* cur = table[i];
        while (cur) {
            if (cur->phone == phone) return cur;
            cur = cur->next;
        }
    }
    return nullptr;
}

// Login Driver
bool DriverList::loginDriver(int &driverID) {
    string phone, password;
    cout << "Enter Phone: "; cin >> phone;
    cout << "Enter Password: "; cin >> password;

    Driver* d = findByPhone(phone);
    if (d && d->password == password) {
        cout << "Login successful! Welcome, " << d->name << "\n";
        driverID = d->id;
        return true;
    }
    cout << "Login failed! Invalid credentials.\n";
    return false;
}

// View driver profile
void DriverList::viewDriverProfile(int driverID) {
    Driver* d = getDriver(driverID);
    if (!d) { cout << "Driver not found!\n"; return; }

    cout << "------ Driver Profile ------\n";
    cout << "ID: " << d->id << "\n";
    cout << "Name: " << d->name << "\n";
    cout << "Phone: " << d->phone << "\n";
    cout << "CNIC: " << d->cnic << "\n";
    cout << "Age: " << d->age << "\n";
    cout << "Rating: " << fixed << setprecision(2) << d->rating << "\n";
    cout << "Status: " << d->status << "\n";
    cout << "Fare per KM: " << fixed << setprecision(2) << d->farePerKm << "\n";
    cout << "Wallet: " << fixed << setprecision(2) << d->wallet << "\n";

    // ⭐ Display linked vehicle
    if (vehicles != nullptr) {
        cout << "\n--- VEHICLE INFO ---\n";
        vehicles->displayVehicle(driverID);
    }
}

// Update fare
void DriverList::updateFare(int driverID, double newFare) {
    Driver* d = getDriver(driverID);
    if (!d) { cout << "Driver not found!\n"; return; }
    d->farePerKm = newFare;
    saveToFile("Driver/drivers.txt");
    cout << "Fare updated.\n";
}

// Accept ride (placeholder)
void DriverList::acceptRide(int driverID) {
    Driver* d = getDriver(driverID);
    if (!d) { cout << "Driver not found!\n"; return; }

    d->status = "Busy";
    saveToFile("Driver/drivers.txt");
    cout << "Ride Accepted.\n";
}

// DELETE driver (plus delete linked vehicle)
void DriverList::deleteDriver(int driverID) {
    int idx = hashFunc(driverID);
    Driver* cur = table[idx];
    Driver* prev = nullptr;

    while (cur) {
        if (cur->id == driverID) {
            // delete linked vehicle
            if (vehicles != nullptr) {
                vehicles->deleteVehicle(driverID);
            }

            if (prev == nullptr) {
                table[idx] = cur->next;
            } else {
                prev->next = cur->next;
            }
            delete cur;
            saveToFile("Driver/drivers.txt");
            cout << "Driver deleted successfully!\n";
            return;
        }
        prev = cur;
        cur = cur->next;
    }
    

    // fallback full scan (if hash mismatch)
    for (int i = 0; i < TABLE_SIZE; ++i) {
        cur = table[i];
        prev = nullptr;
        while (cur) {
            if (cur->id == driverID) {
                if (vehicles != nullptr) vehicles->deleteVehicle(driverID);
                if (prev == nullptr) table[i] = cur->next;
                else prev->next = cur->next;
                delete cur;
                saveToFile("Driver/drivers.txt");
                cout << "Driver deleted successfully!\n";
                return;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    cout << "Driver not found!\n";
}



Driver* DriverList::getDriver(int driverID) {
    int idx = hashFunc(driverID);
    Driver* cur = table[idx];
    while (cur) {
        if (cur->id == driverID) return cur;
        cur = cur->next;
    }

    for (int i = 0; i < TABLE_SIZE; ++i) {
        cur = table[i];
        while (cur) {
            if (cur->id == driverID) return cur;
            cur = cur->next;
        }
    }
    return nullptr;
} // PRINT ALL Drivers (for Admin)
void DriverList::printAll() {
    cout << "\n===== ALL DRIVERS =====\n";

    for (int i = 0; i < TABLE_SIZE; ++i) {
        Driver* cur = table[i];
        while (cur) {
            cout << "ID: " << cur->id
                 << " | Name: " << cur->name
                 << " | Phone: " << cur->phone
                 << " | CNIC: " << cur->cnic
                 << " | Fare/km: " << fixed << setprecision(2) << cur->farePerKm
                 << " | Status: " << cur->status
                 << " | Wallet: " << fixed << setprecision(2) << cur->wallet
                 << "\n";
            cur = cur->next;
        }
    }
}

