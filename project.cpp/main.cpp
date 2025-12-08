#include <iostream>
#include <iomanip>

#include "Rider/Rider.h"
#include "Driver/Driver.h"
#include "Vehicle/Vehicle.h"

using namespace std;

// Global objects
RiderList riders;
DriverList drivers;
VehicleList vehicles;   // vehicle module

// Logged-in user IDs
int currentRiderID = -1;
int currentDriverID = -1;

// Function declarations
void riderMenu();
void registerRider();
void loginRider();
void addWallet();

void driverMenu();
void registerDriver();
void loginDriver();
void updateDriverFare();
void driverAcceptRide();

void adminMenu() {
    cout << "[Admin module placeholder]\n";
    system("pause");
}

int main() {

    // Load files
    riders.loadFromFile("Rider/riders.txt");
    drivers.loadFromFile("Driver/drivers.txt");
    vehicles.loadFromFile("Vehicle/vehicles.txt");

    // Link modules
    drivers.linkVehicleModule(&vehicles);

    int choice;
    do {
        system("cls");
        cout << "----- Smart Ride Sharing System -----\n";
        cout << "1. Rider Login / Register\n";
        cout << "2. Driver Login / Register\n";
        cout << "3. Admin (placeholder)\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: riderMenu(); break;
            case 2: driverMenu(); break;
            case 3: adminMenu(); break;
            case 0: cout << "Exiting...\n"; break;
            default:
                cout << "Invalid choice!\n";
                system("pause");
        }
    } while(choice != 0);

    return 0;
}

/* ================== RIDER MENU ================== */

void riderMenu() {
    int choice;
    do {
        system("cls");
        cout << "----- Rider Panel -----\n";

        if(currentRiderID != -1) {
            Rider* r = riders.getRider(currentRiderID);
            if(r) {
                cout << "Logged in as: " << r->name 
                     << " | Wallet: " << fixed << setprecision(2) << r->wallet << "\n";
            }
        }

        cout << "1. Register Rider\n";
        cout << "2. Login Rider\n";
        cout << "3. Book Ride (placeholder)\n";
        cout << "4. View Ride History (placeholder)\n";
        cout << "5. SOS / Emergency (placeholder)\n";
        cout << "6. Add Money to Wallet\n";
        cout << "7. Delete Rider Account\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: registerRider(); break;
            case 2: loginRider(); break;
            case 3: cout << "[Placeholder] Book Ride\n"; system("pause"); break;
            case 4: cout << "[Placeholder] Ride History\n"; system("pause"); break;
            case 5: cout << "[Placeholder] SOS\n"; system("pause"); break;
            case 6: addWallet(); break;
            case 7:
                if (currentRiderID == -1) {
                    cout << "Please login first!\n";
                } else {
                    riders.deleteRider(currentRiderID);
                    currentRiderID = -1;
                }
                system("pause");
                break;
            case 0:
                cout << "Logging out...\n";
                currentRiderID = -1;
                system("pause");
                break;
            default: cout << "Invalid choice!\n"; system("pause");
        }
    } while(choice != 0);
}

void registerRider() { 
    riders.registerRider();
    system("pause");
}

void loginRider() {
    int riderID;
    if(riders.loginRider(riderID)) {
        currentRiderID = riderID;
        riders.viewRiderProfile(riderID);
        system("pause");
    }
}

void addWallet() {
    if(currentRiderID == -1) {
        cout << "Please login first!\n"; 
        system("pause"); 
        return;
    }
    double amt;
    cout << "Enter amount to add: "; 
    cin >> amt;
    riders.addWallet(currentRiderID, amt);
    system("pause");
}

/* ================== DRIVER MENU ================== */

void driverMenu() {
    int choice;
    do {
        system("cls");
        cout << "----- Driver Panel -----\n";

        if(currentDriverID != -1) {
            Driver* d = drivers.getDriver(currentDriverID);
            if(d) {
                cout << "Logged in as: " << d->name
                     << " | Fare/km: " << fixed << setprecision(2) << d->farePerKm
                     << " | Wallet: " << d->wallet << "\n";
            }
        }

        cout << "1. Register Driver\n";
        cout << "2. Login Driver\n";
        cout << "3. Update Fare Per KM\n";
        cout << "4. Accept Ride (placeholder)\n";
        cout << "5. Delete Driver Account\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: registerDriver(); break;
            case 2: loginDriver(); break;
            case 3: updateDriverFare(); break;
            case 4: driverAcceptRide(); break;
            case 5:
                if (currentDriverID == -1) {
                    cout << "Please login first!\n";
                } else {
                    drivers.deleteDriver(currentDriverID);
                    currentDriverID = -1;
                }
                system("pause");
                break;
            case 0: 
                cout << "Logging out...\n";
                currentDriverID = -1;
                system("pause");
                break;
            default:
                cout << "Invalid choice!\n"; 
                system("pause");
        }

    } while(choice != 0);
}

void registerDriver() {
    drivers.registerDriver();
    system("pause");
}

void loginDriver() {
    int driverID;
    if(drivers.loginDriver(driverID)) {
        currentDriverID = driverID;
        drivers.viewDriverProfile(driverID);
        system("pause");
    }
}

void updateDriverFare() {
    if(currentDriverID == -1) {
        cout << "Please login first!\n"; system("pause"); return;
    }
    double newFare;
    cout << "Enter new fare per KM: ";
    cin >> newFare;
    drivers.updateFare(currentDriverID, newFare);
    system("pause");
}

void driverAcceptRide() {
    if(currentDriverID == -1) {
        cout << "Please login first!\n"; system("pause"); return;
    }
    drivers.acceptRide(currentDriverID);
    system("pause");
}
