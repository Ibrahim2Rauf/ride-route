#include "Driver.h"          // Driver aur DriverList ki definitions
#include <iostream>         
#include <fstream>        
#include <sstream>          
#include <iomanip>          
using namespace std;

const int DriverList::TABLE_SIZE;   // hash table ka fixed size

// Constructor
DriverList::DriverList() {
    for (int i = 0; i < TABLE_SIZE; ++i)
        table[i] = nullptr;         // hash table ke sab buckets empty kiye
    vehicles = nullptr;             // vehicle module ka pointer start mein null
}

// Link vehicle module
void DriverList::linkVehicleModule(VehicleList* v) {
    vehicles = v;                   // driver module ko vehicle module se link kiya
}

// Destructor
DriverList::~DriverList() {
    for (int i = 0; i < TABLE_SIZE; ++i) {   // poori hash table traverse
        Driver* cur = table[i];
        while (cur) {               // har bucket ki linked list delete
            Driver* t = cur;
            cur = cur->next;
            delete t;               
        }
        table[i] = nullptr;         // bucket reset
    }
}

// Simple hash function (by id)
int DriverList::hashFunc(int id) {
    return id % TABLE_SIZE;         // id ko table size se mod karke index nikala
}

// Generate unique ID (scan entire table for max id)
int DriverList::generateID() {
    int maxID = 0;                  // max id track karne ke liye
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Driver* cur = table[i];
        while (cur) {               // poori hash table scan
            if (cur->id > maxID)
                maxID = cur->id;    // sab se bari id save
            cur = cur->next;
        }
    }
    return maxID + 1;               // new unique id return
}

// Load drivers from file (CSV per line)
void DriverList::loadFromFile(const string& filename) {
    ifstream fin(filename);         // file open reading ke liye
    if (!fin) return;               // file na mile to return

    string line;
    while (getline(fin, line)) {    // har line read
        if (line.size() == 0) continue; // empty line skip
        stringstream ss(line);
        Driver* d = new Driver;     // naya driver object
        string token;

        getline(ss, token, ','); d->id = stoi(token);   
        getline(ss, d->name, ',');                       
        getline(ss, d->phone, ',');                      
        getline(ss, d->password, ',');                   
        getline(ss, d->cnic, ',');                       
        getline(ss, token, ','); d->age = stoi(token);   
        getline(ss, token, ','); d->rating = stod(token);
        getline(ss, d->status, ',');                    
        getline(ss, token, ','); d->farePerKm = stod(token);
        getline(ss, token, ','); d->wallet = stod(token);   
        getline(ss, d->vehicleNumber);                 

        int idx = hashFunc(d->id);   // hash index nikala
        d->next = table[idx];        // front insertion in hash bucket
        table[idx] = d;
    }
    fin.close();                     // file close
}

// Save drivers to file (CSV)
void DriverList::saveToFile(const string& filename) {
    ofstream fout(filename);         // file open writing ke liye
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Driver* cur = table[i];
        while (cur) {               // har driver ka data save
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
    fout.close();                    // file close
}

// Register Driver
void DriverList::registerDriver() {
    Driver* d = new Driver;          // naya driver
    d->id = generateID();            // unique id assign

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, d->name);           // full name input

    cout << "Enter Phone: ";
    cin >> d->phone;

    cout << "Enter Password: ";
    cin >> d->password;

    cout << "Enter CNIC: ";
    cin >> d->cnic;

    cout << "Enter Age: ";
    cin >> d->age;

    d->rating = 4.5;                 // default rating
    d->status = "Available";         // default status

    cout << "Enter Fare per KM: ";
    cin >> d->farePerKm;

    d->wallet = 500.0;               // default wallet amount

    //  Driver ke sath vehicle automatically add hoti hai
    if (vehicles != nullptr) {
        vehicles->addVehicle(d->id);
    }

    d->vehicleNumber = "AUTO";       // compatibility ke liye

    int idx = hashFunc(d->id);       // hash index
    d->next = table[idx];            // hash table insertion
    table[idx] = d;

    saveToFile("Driver/drivers.txt"); // data file mein save
    cout << "Driver registered successfully! Driver ID: " << d->id << "\n";
}

// Find driver by phone
Driver* DriverList::findByPhone(const string& phone) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Driver* cur = table[i];
        while (cur) {
            if (cur->phone == phone)
                return cur;          // phone match ho gaya
            cur = cur->next;
        }
    }
    return nullptr;                  // agar na mile
}

// Login Driver
bool DriverList::loginDriver(int &driverID) {
    string phone, password;

    cout << "Enter Phone: ";
    cin >> phone;

    cout << "Enter Password: ";
    cin >> password;

    Driver* d = findByPhone(phone);  // phone se driver find
    if (d && d->password == password) {
        cout << "Login successful! Welcome, " << d->name << "\n";
        driverID = d->id;            // logged in driver ki id
        return true;
    }

    cout << "Login failed! Invalid credentials.\n";
    return false;
}

// View driver profile
void DriverList::viewDriverProfile(int driverID) {
    Driver* d = getDriver(driverID); // id se driver nikala
    if (!d) {
        cout << "Driver not found!\n";
        return;
    }

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

    //  linked vehicle ki info show
    if (vehicles != nullptr) {
        cout << "\n--- VEHICLE INFO ---\n";
        vehicles->displayVehicle(driverID);
    }
}

// Update fare
void DriverList::updateFare(int driverID, double newFare) {
    Driver* d = getDriver(driverID);
    if (!d) {
        cout << "Driver not found!\n";
        return;
    }

    d->farePerKm = newFare;          // fare update
    saveToFile("Driver/drivers.txt");
    cout << "Fare updated.\n";
}

// Accept ride
void DriverList::acceptRide(int driverID) {
    Driver* d = getDriver(driverID);
    if (!d) {
        cout << "Driver not found!\n";
        return;
    }

    d->status = "Busy";              // driver busy ho gaya
    saveToFile("Driver/drivers.txt");
    cout << "Ride Accepted.\n";
}

// Complete ride
void DriverList::completeRide(int driverID) {
    Driver* d = getDriver(driverID);
    if (!d) return;

    d->status = "Available";         // ride complete hone ke baad available
    saveToFile("Driver/drivers.txt");
}

// DELETE driver (plus delete linked vehicle)
void DriverList::deleteDriver(int driverID) {
    int idx = hashFunc(driverID);    // primary hash index
    Driver* cur = table[idx];
    Driver* prev = nullptr;

    while (cur) {
        if (cur->id == driverID) {
            if (vehicles != nullptr) {
                vehicles->deleteVehicle(driverID); // linked vehicle delete
            }

            if (prev == nullptr)
                table[idx] = cur->next;
            else
                prev->next = cur->next;

            delete cur;              // driver memory free
            saveToFile("Driver/drivers.txt");
            cout << "Driver deleted successfully!\n";
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    // fallback: agar wrong bucket mein ho to full scan
    for (int i = 0; i < TABLE_SIZE; ++i) {
        cur = table[i];
        prev = nullptr;
        while (cur) {
            if (cur->id == driverID) {
                if (vehicles != nullptr)
                    vehicles->deleteVehicle(driverID);

                if (prev == nullptr)
                    table[i] = cur->next;
                else
                    prev->next = cur->next;

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

// Get driver by ID
Driver* DriverList::getDriver(int driverID) {
    int idx = hashFunc(driverID);
    Driver* cur = table[idx];

    while (cur) {
        if (cur->id == driverID)
            return cur;
        cur = cur->next;
    }

    // fallback full scan
    for (int i = 0; i < TABLE_SIZE; ++i) {
        cur = table[i];
        while (cur) {
            if (cur->id == driverID)
                return cur;
            cur = cur->next;
        }
    }
    return nullptr;
}

// PRINT ALL Drivers (for Admin)
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