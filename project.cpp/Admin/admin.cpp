#include "Admin.h"
#include "../Rider/Rider.h"
#include "../Driver/Driver.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

/* ===========================
   ADMIN LOGIN
=========================== */
bool Admin::login() {
    string u, p;
    cout << "Enter admin username: ";
    cin >> u;
    cout << "Enter admin password: ";
    cin >> p;

    if (u == adminUser && p == adminPass) {
        cout << "Admin login successful!\n";
        return true;
    }

    cout << "Invalid admin credentials.\n";
    return false;
}


/* ===========================
   VIEW ALL RIDERS
=========================== */
void Admin::viewAllRiders() {
    ifstream fin("Rider/riders.txt");
    if (!fin) {
        cout << "No rider file found!\n";
        return;
    }

    cout << "\n===== ALL RIDERS =====\n";
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string id, name, phone, pass, wallet;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, phone, ',');
        getline(ss, pass, ',');
        getline(ss, wallet, ',');

        cout << "ID: " << id
             << " | Name: " << name
             << " | Phone: " << phone
             << " | Wallet: " << wallet << "\n";
    }
    fin.close();
}


/* ===========================
   VIEW ALL DRIVERS
=========================== */
void Admin::viewAllDrivers() {
    ifstream fin("Driver/drivers.txt");
    if (!fin) {
        cout << "No driver file found!\n";
        return;
    }

    cout << "\n===== ALL DRIVERS =====\n";

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);

        string id, name, phone, pass, cnic, age, rating, status, fare, wallet, vnum;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, phone, ',');
        getline(ss, pass, ',');
        getline(ss, cnic, ',');
        getline(ss, age, ',');
        getline(ss, rating, ',');
        getline(ss, status, ',');
        getline(ss, fare, ',');
        getline(ss, wallet, ',');
        getline(ss, vnum, ',');

        cout << "ID: " << id
             << " | Name: " << name
             << " | Phone: " << phone
             << " | CNIC: " << cnic
             << " | Age: " << age
             << " | Rating: " << rating
             << " | Status: " << status
             << " | Fare/km: " << fare
             << " | Wallet: " << wallet
             << " | Vehicle No: " << vnum
             << "\n";
    }
    fin.close();
}


/* ===========================
   VIEW ALL VEHICLES
=========================== */
void Admin::viewAllVehicles() {
    ifstream fin("Vehicle/vehicles.txt");
    if (!fin) {
        cout << "No vehicle file found!\n";
        return;
    }

    cout << "\n===== ALL VEHICLES =====\n";

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);

        string vid, did, brand, model, color, plate, type, rating;

        getline(ss, vid, ',');
        getline(ss, did, ',');
        getline(ss, brand, ',');
        getline(ss, model, ',');
        getline(ss, color, ',');
        getline(ss, plate, ',');
        getline(ss, type, ',');
        getline(ss, rating, ',');

        cout << "VehicleID: " << vid
             << " | DriverID: " << did
             << " | " << brand << " " << model
             << " | Color: " << color
             << " | Plate: " << plate
             << " | Type: " << type
             << " | Rating: " << rating
             << "\n";
    }
    fin.close();
}


/* ===========================
   VIEW ALL RIDES (History)
=========================== */
void Admin::viewAllRides() {
    ifstream fin("History/history.txt");
    if (!fin) {
        cout << "No ride history file found!\n";
        return;
    }

    cout << "\n===== ALL RIDE HISTORY =====\n";

    string line;
    while (getline(fin, line)) {
        cout << line << "\n";
    }
    fin.close();
}
void Admin::deleteRider(int riderID, RiderList &riders) {
    riders.deleteRider(riderID);
    cout << "✅ Rider deleted successfully!\n";
}

/* ==========================
   DELETE DRIVER
========================== */
void Admin::deleteDriver(int driverID, DriverList &drivers) {
    drivers.deleteDriver(driverID);
    cout << "✅ Driver deleted successfully!\n";
}