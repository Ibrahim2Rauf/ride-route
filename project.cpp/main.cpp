#include <iostream>
#include <fstream>
#include <string>
#include "Rider.h"
#include "Driver.h"
#include "Vehicle.h"
#include "Graph.h"
#include "Match.h"
#include "Payment.h"
#include "History.h"
#include "Security.h"
#include "Admin.h"
using namespace std;
// Rider functions
void riderMenu();
void registerRider();
void loginRider();
void bookRide();
void viewRideHistory();
void riderSOS();
// Driver functions
void driverMenu();
void registerDriver();
void loginDriver();
void acceptRide();
void updateFare();
void completeRide();
// Admin functions
void adminMenu();
void viewAllRiders();
void viewAllDrivers();
void viewAllVehicles();
void viewAllRides();

int main() {
    int choice;

    do {
        system("cls"); 
        cout << "---------------------------------------\n";
        cout << "        Smart Ride Sharing System      \n";
        cout << "---------------------------------------\n";
        cout << "1. Rider Login / Register\n";
        cout << "2. Driver Login / Register\n";
        cout << "3. Admin Panel\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                riderMenu();  
                break;
            case 2:
                driverMenu(); 
                break;
            case 3:
                adminMenu(); 
                break;
            case 0:
                cout << "Exiting system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
                system("pause");
        }

    } while (choice != 0);
    return 0;
}
void riderMenu() {
    int choice;
    do {
        system("cls");
        cout << "------ Rider Panel ------\n";
        cout << "1. Register Rider\n";
        cout << "2. Login Rider\n";
        cout << "3. Book Ride\n";
        cout << "4. View Ride History\n";
        cout << "5. SOS / Emergency\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerRider(); break;
            case 2: loginRider(); break;
            case 3: bookRide(); break;
            case 4: viewRideHistory(); break;
            case 5: riderSOS(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice. Try again.\n"; system("pause");
        }
    } while (choice != 0);
}
void driverMenu() {
    int choice;
    do {
        system("cls");
        cout << "------ Driver Panel ------\n";
        cout << "1. Register Driver\n";
        cout << "2. Login Driver\n";
        cout << "3. Accept Ride\n";
        cout << "4. Update Fare\n";
        cout << "5. Complete Ride\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerDriver(); break;
            case 2: loginDriver(); break;
            case 3: acceptRide(); break;
            case 4: updateFare(); break;
            case 5: completeRide(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice. Try again.\n"; system("pause");
        }
    } while (choice != 0);
}
void adminMenu() {
    int choice;
    do {
        system("cls");
        cout << "------ Admin Panel ------\n";
        cout << "1. View All Riders\n";
        cout << "2. View All Drivers\n";
        cout << "3. View All Vehicles\n";
        cout << "4. View All Rides\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewAllRiders(); break;
            case 2: viewAllDrivers(); break;
            case 3: viewAllVehicles(); break;
            case 4: viewAllRides(); break;
            case 0: cout << "Exiting admin panel...\n"; break;
            default: cout << "Invalid choice. Try again.\n"; system("pause");
        }
    } while (choice != 0);
}
void registerRider() { cout << "[Placeholder] Register Rider called.\n"; system("pause"); }
void loginRider() { cout << "[Placeholder] Login Rider called.\n"; system("pause"); }
void bookRide() { cout << "[Placeholder] Book Ride called.\n"; system("pause"); }
void viewRideHistory() { cout << "[Placeholder] View Ride History called.\n"; system("pause"); }
void riderSOS() { cout << "[Placeholder] SOS sent!\n"; system("pause"); }

void registerDriver() { cout << "[Placeholder] Register Driver called.\n"; system("pause"); }
void loginDriver() { cout << "[Placeholder] Login Driver called.\n"; system("pause"); }
void acceptRide() { cout << "[Placeholder] Accept Ride called.\n"; system("pause"); }
void updateFare() { cout << "[Placeholder] Update Fare called.\n"; system("pause"); }
void completeRide() { cout << "[Placeholder] Complete Ride called.\n"; system("pause"); }

void viewAllRiders() { cout << "[Placeholder] View All Riders called.\n"; system("pause"); }
void viewAllDrivers() { cout << "[Placeholder] View All Drivers called.\n"; system("pause"); }
void viewAllVehicles() { cout << "[Placeholder] View All Vehicles called.\n"; system("pause"); }
void viewAllRides() { cout << "[Placeholder] View All Rides called.\n"; system("pause"); }

