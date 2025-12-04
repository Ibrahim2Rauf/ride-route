#include <iostream>
#include <iomanip>
#include "Rider/Rider.h"

using namespace std;

RiderList riders;
int currentRiderID = -1; // store logged-in rider

// Function declarations
void riderMenu();
void registerRider();
void loginRider();
void bookRide();
void viewRideHistory();
void riderSOS();
void addWallet();

// Placeholders for other modules
void driverMenu() { cout << "[Driver module placeholder]\n"; system("pause"); }
void adminMenu() { cout << "[Admin module placeholder]\n"; system("pause"); }

int main() {
    riders.loadFromFile("Rider/riders.txt");

    int choice;
    do {
        system("cls");
        cout << "----- Smart Ride Sharing -----\n";
        cout << "1. Rider Login / Register\n";
        cout << "2. Driver (placeholder)\n";
        cout << "3. Admin (placeholder)\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: riderMenu(); break;
            case 2: driverMenu(); break;
            case 3: adminMenu(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n"; system("pause");
        }
    } while(choice != 0);

    return 0;
}

void riderMenu() {
    int choice;
    do {
        system("cls");
        cout << "----- Rider Panel -----\n";

        if(currentRiderID != -1) {
            Rider* r = riders.getRider(currentRiderID);
            if(r) cout << "Logged in as: " << r->name 
                        << " | Wallet: " << fixed << setprecision(2) << r->wallet << "\n";
        }

        cout << "1. Register Rider\n";
        cout << "2. Login Rider\n";
        cout << "3. Book Ride (placeholder)\n";
        cout << "4. View Ride History (placeholder)\n";
        cout << "5. SOS / Emergency (placeholder)\n";
        cout << "6. Add Money to Wallet\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: registerRider(); break;
            case 2: loginRider(); break;
            case 3: bookRide(); break;
            case 4: viewRideHistory(); break;
            case 5: riderSOS(); break;
            case 6: addWallet(); break;
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
    // get the last added rider (the one we just registered)
    int lastID = riders.generateID() - 1; // because generateID gives next ID
    currentRiderID = lastID;
    cout << "\nAuto-login successful!\n";
    riders.viewRiderProfile(currentRiderID); // shows wallet immediately
}


void loginRider() {
    int riderID;
    if(riders.loginRider(riderID)) {
        currentRiderID = riderID;
        riders.viewRiderProfile(riderID);
        system("pause");
    }
}

void bookRide() { cout << "[Placeholder] Book Ride\n"; system("pause"); }
void viewRideHistory() { cout << "[Placeholder] Ride History\n"; system("pause"); }
void riderSOS() { cout << "[Placeholder] SOS\n"; system("pause"); }

void addWallet() {
    if(currentRiderID == -1) {
        cout << "Please login first!\n"; system("pause"); return;
    }
    double amt;
    cout << "Enter amount to add: "; cin >> amt;
    riders.addWallet(currentRiderID, amt);
    system("pause");
}
