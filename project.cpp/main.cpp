#include <iostream>
#include <iomanip>
#include <windows.h>

#include "Rider/Rider.h"
#include "Driver/Driver.h"
#include "Vehicle/Vehicle.h"
#include "Graph/Graph.h"
#include "Match/Match.h"
#include "Payment/Payment.h"
#include "History/History.h"
#include "Security/Security.h"
#include "Admin/Admin.h"

using namespace std;

/* ================= COLOR CODES ================= */
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define UNDERLINE "\033[4m"

#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

/* ================= ENABLE COLORS FOR WINDOWS ================= */
void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    
    // Set UTF-8 encoding
    SetConsoleOutputCP(CP_UTF8);
}

/* ================= GLOBAL OBJECTS ================= */

RiderList riders;
DriverList drivers;
VehicleList vehicles;
Graph graph;
MatchEngine matcher;
PaymentList payments;
HistoryList history;
Security security;
Admin admin;

/* ================= SESSION VARIABLES ================= */

int currentRiderID = -1;
int currentDriverID = -1;

/* ================= FUNCTION DECLARATIONS ================= */

void riderMenu();
void driverMenu();
void adminMenu();
void bookRide();

/* ========================= MAIN ========================= */

int main() {
    // Enable ANSI colors for Windows
    enableANSI();
    
    // Load data
    riders.loadFromFile("Rider/riders.txt");
    drivers.loadFromFile("Driver/drivers.txt");
    vehicles.loadFromFile("Vehicle/vehicles.txt");
    payments.loadFromFile("Payment/payments.txt");
    history.loadFromFile("History/history.txt");

    // Link modules
    drivers.linkVehicleModule(&vehicles);
    matcher.linkGraph(&graph);
    matcher.linkDrivers(&drivers);

    // Setup graph
    graph.setNodeCount(10);
    graph.addEdge(1,2,5); graph.addEdge(2,1,5);
    graph.addEdge(2,3,4); graph.addEdge(3,2,4);
    graph.addEdge(3,4,6); graph.addEdge(4,3,6);
    graph.addEdge(4,5,3); graph.addEdge(5,4,3);

    int choice;
    do {
        system("cls");
        
        // Beautiful header with gradient effect
        cout << "\n\n";
        cout << BOLD << CYAN << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║" << YELLOW << "          🚗  SMART RIDE SHARING SYSTEM  🚗        " << CYAN << "║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║" << WHITE << "              🌟 Premium Edition v2.0 🌟           " << CYAN << "║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        
        cout << "\n";
        cout << GREEN << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << BOLD << "1." << RESET << GREEN << "  🧑  " << BOLD << "Rider Portal" << RESET << GREEN << "  - Book & Manage Rides     ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << BOLD << "2." << RESET << GREEN << "  🚙  " << BOLD << "Driver Portal" << RESET << GREEN << " - Accept & Earn           ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << BOLD << "3." << RESET << GREEN << "  👑  " << BOLD << "Admin Portal" << RESET << GREEN << "  - System Management       ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << BOLD << "0." << RESET << GREEN << "  ❌  " << BOLD << "Exit System" << RESET << GREEN << "  - Close Application        ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        
        cout << "\n" << CYAN << "          ➤  " << RESET << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: riderMenu(); break;
            case 2: driverMenu(); break;
            case 3: adminMenu(); break;
            case 0: 
                cout << "\n\n";
                cout << YELLOW << "          ╔══════════════════════════════════════════════════╗\n";
                cout << "          ║                                                  ║\n";
                cout << "          ║" << BOLD << "          ⚡ SYSTEM SHUTTING DOWN... ⚡             " << RESET << YELLOW << "║\n";
                cout << "          ║                                                  ║\n";
                cout << "          ║          Thank you for using our service!       ║\n";
                cout << "          ║                                                  ║\n";
                cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
                Sleep(1500);
                break;
            default: 
                cout << "\n" << RED << BOLD << "          ❌ ERROR: Invalid choice! Please try again.\n" << RESET; 
                Sleep(1000);
        }
    } while(choice != 0);

    return 0;
}

/* ========================= RIDER ========================= */

void riderMenu() {
    int ch;
    do {
        system("cls");
        cout << "\n\n";
        cout << BOLD << MAGENTA << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║" << YELLOW << "          🧑  RIDER CONTROL PANEL  🧑                " << MAGENTA << "║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        
        cout << "\n";
        cout << CYAN << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "1." << RESET << CYAN << "  📝  Register New Account                      ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "2." << RESET << CYAN << "  🔐  Login (OTP Secured)                       ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "3." << RESET << CYAN << "  🚕  Book a Ride                               ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "4." << RESET << CYAN << "  💳  View Payment History                      ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "5." << RESET << CYAN << "  📜  View Ride History                         ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "6." << RESET << CYAN << "  💰  Add Money to Wallet                       ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "0." << RESET << CYAN << "  🚪  Logout & Return                           ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        
        cout << "\n" << YELLOW << "          ➤  " << RESET << "Enter your choice: ";
        cin >> ch;

        switch(ch) {
            case 1:
                cout << "\n" << GREEN << BOLD << "          ═══════════════ REGISTRATION ═══════════════\n\n" << RESET;
                riders.registerRider();
                cout << "\n" << GREEN << BOLD << "          ✅ Registration process completed!\n" << RESET;
                Sleep(1000);
                system("pause");
                break;

            case 2: {
                if (security.isLocked()) {
                    cout << "\n" << RED << BOLD << "          ╔══════════════════════════════════════════════════╗\n";
                    cout << "          ║                                                  ║\n";
                    cout << "          ║    🔒  ACCOUNT LOCKED - SECURITY VIOLATION!     ║\n";
                    cout << "          ║                                                  ║\n";
                    cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
                    Sleep(1500);
                    system("pause");
                    break;
                }

                int id;
                if (riders.loginRider(id)) {
                    int otp = security.generateOTP();
                    int entered;
                    cout << "\n" << YELLOW << BOLD << "          🔑 Security OTP has been generated!\n\n" << RESET;
                    cout << YELLOW << "          ➤  " << RESET << "Enter OTP code: ";
                    cin >> entered;

                    if (security.verifyOTP(entered)) {
                        currentRiderID = id;
                        cout << "\n" << GREEN << BOLD << "          ╔══════════════════════════════════════════════════╗\n";
                        cout << "          ║                                                  ║\n";
                        cout << "          ║         ✅  LOGIN SUCCESSFUL! WELCOME BACK       ║\n";
                        cout << "          ║                                                  ║\n";
                        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
                        Sleep(1000);
                    } else {
                        cout << "\n" << RED << BOLD << "          ❌ Invalid OTP! Access denied.\n" << RESET;
                        Sleep(1000);
                    }
                }
                system("pause");
            } break;

            case 3:
                bookRide();
                break;

            case 4:
                cout << "\n" << CYAN << BOLD << "          ═══════════════ PAYMENT HISTORY ═══════════════\n\n" << RESET;
                payments.viewRiderPayments(currentRiderID);
                cout << "\n";
                system("pause");
                break;
                
            case 5:
                cout << "\n" << CYAN << BOLD << "          ═══════════════ RIDE HISTORY ═══════════════\n\n" << RESET;
                history.viewRiderHistory(currentRiderID);
                cout << "\n";
                system("pause");
                break;

            case 6: {
                double amt;
                cout << "\n" << GREEN << BOLD << "          ═══════════════ ADD WALLET MONEY ═══════════════\n\n" << RESET;
                cout << GREEN << "          💵  " << RESET << "Enter amount (PKR): ";
                cin >> amt;
                riders.addWallet(currentRiderID, amt);
                cout << "\n" << GREEN << BOLD << "          ╔══════════════════════════════════════════════════╗\n";
                cout << "          ║                                                  ║\n";
                cout << "          ║         ✅  WALLET UPDATED SUCCESSFULLY!         ║\n";
                cout << "          ║                                                  ║\n";
                cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
                Sleep(1000);
                system("pause");
            } break;
        }
    } while(ch != 0);

    currentRiderID = -1;
}

/* ========================= DRIVER ========================= */

void driverMenu() {
    int ch;
    do {
        system("cls");
        cout << "\n\n";
        cout << BOLD << BLUE << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║" << YELLOW << "          🚙  DRIVER CONTROL PANEL  🚙               " << BLUE << "║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        
        cout << "\n";
        cout << GREEN << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "1." << RESET << GREEN << "  📝  Register as Driver                        ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "2." << RESET << GREEN << "  🔑  Login to System                           ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "3." << RESET << GREEN << "  💳  View Earnings & Payments                  ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "4." << RESET << GREEN << "  📜  View Ride History                         ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "0." << RESET << GREEN << "  🚪  Logout & Return                           ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        
        cout << "\n" << YELLOW << "          ➤  " << RESET << "Enter your choice: ";
        cin >> ch;

        switch(ch) {
            case 1:
                cout << "\n" << GREEN << BOLD << "          ═══════════════ DRIVER REGISTRATION ═══════════════\n\n" << RESET;
                drivers.registerDriver();
                cout << "\n" << GREEN << BOLD << "          ✅ Registration completed!\n" << RESET;
                Sleep(1000);
                system("pause");
                break;

            case 2: {
                int id;
                if(drivers.loginDriver(id)) {
                    currentDriverID = id;
                    int loc;
                    cout << "\n" << CYAN << "          📍  " << RESET << "Enter your current location (node): ";
                    cin >> loc;
                    matcher.setDriverLocation(id, loc);
                    cout << "\n" << GREEN << BOLD << "          ╔══════════════════════════════════════════════════╗\n";
                    cout << "          ║                                                  ║\n";
                    cout << "          ║         ✅  LOGIN SUCCESSFUL! READY TO DRIVE     ║\n";
                    cout << "          ║                                                  ║\n";
                    cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
                    Sleep(1000);
                }
                system("pause");
            } break;

            case 3:
                cout << "\n" << CYAN << BOLD << "          ═══════════════ EARNINGS REPORT ═══════════════\n\n" << RESET;
                payments.viewDriverPayments(currentDriverID);
                cout << "\n";
                system("pause");
                break;

            case 4:
                cout << "\n" << CYAN << BOLD << "          ═══════════════ RIDE HISTORY ═══════════════\n\n" << RESET;
                history.viewDriverHistory(currentDriverID);
                cout << "\n";
                system("pause");
                break;
        }
    } while(ch != 0);

    currentDriverID = -1;
}

/* ========================= BOOK RIDE ========================= */

void bookRide() {
    if(currentRiderID == -1) {
        cout << "\n" << RED << BOLD << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║         ❌  LOGIN REQUIRED TO BOOK RIDES!        ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        Sleep(1500);
        system("pause");
        return;
    }

    int src, dst;
    cout << "\n" << CYAN << BOLD << "          ═══════════════ BOOK YOUR RIDE ═══════════════\n\n" << RESET;
    cout << CYAN << "          📍  " << RESET << "Enter pickup location (node): "; 
    cin >> src;
    cout << CYAN << "          📍  " << RESET << "Enter drop location (node): "; 
    cin >> dst;

    cout << "\n" << YELLOW << "          🔍 Searching for available drivers...\n" << RESET;
    Sleep(1000);

    MatchResult result;
    int driverID = matcher.matchRide(currentRiderID, src, dst, true, result);

    if(driverID == -1) {
        cout << "\n" << RED << BOLD << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║       ❌  NO DRIVERS AVAILABLE RIGHT NOW!        ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║          Please try again in a few minutes      ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        Sleep(1500);
        system("pause");
        return;
    }

    cout << "\n" << BOLD << GREEN << "          ╔══════════════════════════════════════════════════╗\n";
    cout << "          ║                                                  ║\n";
    cout << "          ║" << YELLOW << "           ✅  DRIVER FOUND! RIDE DETAILS              " << GREEN << "║\n";
    cout << "          ║                                                  ║\n";
    cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;

    cout << "\n";
    cout << YELLOW << "          🚗  Driver ID       : " << RESET << WHITE << BOLD << driverID << RESET << "\n";
    cout << YELLOW << "          📏  Distance        : " << RESET << WHITE << BOLD << result.distance << " km" << RESET << "\n";

    Driver* d = drivers.getDriver(driverID);
    cout << YELLOW << "          💵  Rate per km     : " << RESET << WHITE << BOLD << d->farePerKm << " PKR" << RESET << "\n";

    cout << "\n" << GREEN << BOLD << "          ╔══════════════════════════════════════════════════╗\n";
    cout << "          ║                                                  ║\n";
    cout << "          ║    💰  TOTAL FARE: " << WHITE << BOLD << fixed << setprecision(2) << result.totalCost << " PKR" << GREEN << "                 ║\n";
    cout << "          ║                                                  ║\n";
    cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;

    cout << "\n";
    cout << CYAN << "          ╔══════════════════════════════════════════════════╗\n";
    cout << "          ║                                                  ║\n";
    cout << "          ║  " << GREEN << BOLD << "Press 1" << RESET << CYAN << "  ➤  ✅ CONFIRM & BOOK THIS RIDE          ║\n";
    cout << "          ║                                                  ║\n";
    cout << "          ║  " << RED << BOLD << "Press 9" << RESET << CYAN << "  ➤  🆘 SEND EMERGENCY SOS ALERT         ║\n";
    cout << "          ║                                                  ║\n";
    cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
    
    cout << "\n" << YELLOW << "          ➤  " << RESET;
    int ch; 
    cin >> ch;

    if (ch == 9) {
        cout << "\n" << RED << BOLD << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║         🆘  EMERGENCY SOS ALERT SENT!            ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║      Emergency services have been notified      ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        security.sendSOS("Rider", "On Route");
        Sleep(2000);
        system("pause");
        return;
    }

    cout << "\n" << YELLOW << "          ⏳ Processing your payment...\n" << RESET;
    Sleep(800);

    payments.makePayment(currentRiderID, driverID, result.totalCost, riders, drivers);
    history.addHistory(currentRiderID, driverID, src, dst, result.totalCost, "Completed");

    drivers.acceptRide(driverID);
    
    cout << "\n" << YELLOW << "          🚗 Driver is on the way...\n" << RESET;
    Sleep(1000);
    
    cout << "\n" << YELLOW << "          🛣️  Ride in progress...\n" << RESET;
    Sleep(1500);
    
    drivers.completeRide(driverID);

    cout << "\n" << GREEN << BOLD << "          ╔══════════════════════════════════════════════════╗\n";
    cout << "          ║                                                  ║\n";
    cout << "          ║         ✅  RIDE COMPLETED SUCCESSFULLY!         ║\n";
    cout << "          ║                                                  ║\n";
    cout << "          ║          Thank you for riding with us! 🎉       ║\n";
    cout << "          ║                                                  ║\n";
    cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;

    Sleep(2000);
    system("pause");
}

/* ========================= ADMIN ========================= */

void adminMenu() {
    if(!admin.login()) {
        system("pause");
        return;
    }

    int ch;
    do {
        system("cls");
        cout << "\n\n";
        cout << BOLD << RED << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║" << YELLOW << "          👑  ADMIN CONTROL PANEL  👑                " << RED << "║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        
        cout << "\n";
        cout << YELLOW << "          ╔══════════════════════════════════════════════════╗\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "1." << RESET << YELLOW << "  👥  View All Registered Riders                ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "2." << RESET << YELLOW << "  🚗  View All Active Drivers                   ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "3." << RESET << YELLOW << "  🚙  View All Vehicles Database                ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "4." << RESET << YELLOW << "  💳  View Complete Payment Records             ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "5." << RESET << YELLOW << "  📜  View Complete Ride History                ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ║  " << WHITE << "0." << RESET << YELLOW << "  🚪  Exit Admin Panel                          ║\n";
        cout << "          ║                                                  ║\n";
        cout << "          ╚══════════════════════════════════════════════════╝\n" << RESET;
        
        cout << "\n" << CYAN << "          ➤  " << RESET << "Enter your choice: ";
        cin >> ch;

        switch(ch) {
            case 1: 
                cout << "\n" << CYAN << BOLD << "          ═══════════════ ALL RIDERS DATABASE ═══════════════\n\n" << RESET;
                admin.viewAllRiders(); 
                break;
            case 2: 
                cout << "\n" << CYAN << BOLD << "          ═══════════════ ALL DRIVERS DATABASE ═══════════════\n\n" << RESET;
                admin.viewAllDrivers(); 
                break;
            case 3: 
                cout << "\n" << CYAN << BOLD << "          ═══════════════ VEHICLES DATABASE ═══════════════\n\n" << RESET;
                admin.viewAllVehicles(); 
                break;
            case 4: 
                cout << "\n" << CYAN << BOLD << "          ═══════════════ PAYMENT RECORDS ═══════════════\n\n" << RESET;
                payments.adminViewAllPayments(); 
                break;
            case 5: 
                cout << "\n" << CYAN << BOLD << "          ═══════════════ COMPLETE RIDE HISTORY ═══════════════\n\n" << RESET;
                admin.viewAllRides(); 
                break;
        }
        cout << "\n";
        system("pause");
    } while(ch != 0);
}