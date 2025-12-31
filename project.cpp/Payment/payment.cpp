#include "Payment.h"            
#include "../Rider/Rider.h"     
#include "../Driver/Driver.h"   
#include <iostream>             
#include <fstream>              
#include <sstream>              
#include <ctime>                
#include <iomanip>              

using namespace std;           


// Constructor
PaymentList::PaymentList() {
    head = nullptr;           // Linked list initially empty
}


// Destructor
PaymentList::~PaymentList() {
    Payment* temp;
    while (head) {            // Jab tak list end na ho
        temp = head;          // Current node ko temporary store karlo
        head = head->next;    // Head ko next node pe shift karo
        delete temp;          // Purani node delete karo
    }
}

// ========================
// Generate unique payment ID
// ========================
int PaymentList::generateID() {
    int mx = 0;
    Payment* p = head;
    while (p) {               // Saari payments scan karo
        if (p->paymentID > mx) mx = p->paymentID; // Max ID find karo
        p = p->next;
    }
    return mx + 1;            // Naya ID max+1
}

// ========================
// Load payments from file
// ========================
void PaymentList::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin) return;         // File na mile to return

    string line;
    while (getline(fin, line)) {  // File line by line read karo
        stringstream ss(line);
        Payment* p = new Payment;

        // CSV parsing
        getline(ss, line, ','); p->paymentID = stoi(line);
        getline(ss, line, ','); p->riderID = stoi(line);
        getline(ss, line, ','); p->driverID = stoi(line);
        getline(ss, line, ','); p->amount = stod(line);
        getline(ss, p->datetime); // datetime last field

        // Insert at front of linked list
        p->next = head;
        head = p;
    }

    fin.close();              // File close
}

// ========================
// Save payments to file
// ========================
void PaymentList::saveToFile(const string& filename) {
    ofstream fout(filename);

    Payment* p = head;
    while (p) {               // Linked list traverse karo
        fout << p->paymentID << ","
             << p->riderID << ","
             << p->driverID << ","
             << fixed << setprecision(2) << p->amount << ","
             << p->datetime << "\n";
        p = p->next;
    }

    fout.close();             // File close
}

// ========================
// Make a payment from rider to driver
// ========================
void PaymentList::makePayment(int riderID, int driverID, double amount,
                              RiderList &riders, DriverList &drivers) {

    Rider* r = riders.getRider(riderID);     // Rider find karo
    Driver* d = drivers.getDriver(driverID); // Driver find karo

    if (!r || !d) {                          // Check valid
        cout << "Error: Invalid rider or driver!\n";
        return;
    }

    if (r->wallet < amount) {                // Check balance
        cout << "Payment failed! Rider does not have enough balance.\n";
        return;
    }

    // ========================
    // Money transfer
    // ========================
    r->wallet -= amount;                     // Rider wallet reduce
    d->wallet += amount;                     // Driver wallet increase

    // Update files
    riders.saveToFile("Rider/riders.txt");
    drivers.saveToFile("Driver/drivers.txt");

    // ========================
    // Create payment record
    // ========================
    Payment* p = new Payment;
    p->paymentID = generateID();
    p->riderID = riderID;
    p->driverID = driverID;
    p->amount = amount;

    // ========================
    // Time stamp
    // ========================
    time_t now = time(0);
    p->datetime = ctime(&now);               // Convert to string
    p->datetime.pop_back();                  // Remove newline character

    // Insert into linked list
    p->next = head;
    head = p;

    saveToFile("Payment/payments.txt");      // Save payment file

    cout << "Payment successful!\n";
}

// ========================
// View payment history of a rider
// ========================
void PaymentList::viewRiderPayments(int riderID) {
    Payment* p = head;
    cout << "--- Rider Payment History ---\n";
    while (p) {
        if (p->riderID == riderID)          // Only this rider's payments
            cout << "Paid: " << p->amount << " to Driver " << p->driverID
                 << " | On: " << p->datetime << "\n";
        p = p->next;
    }
}

// ========================
// View payment history of a driver
// ========================
void PaymentList::viewDriverPayments(int driverID) {
    Payment* p = head;
    cout << "--- Driver Payment History ---\n";
    while (p) {
        if (p->driverID == driverID)         // Only this driver's payments
            cout << "Received: " << p->amount << " from Rider " << p->riderID
                 << " | On: " << p->datetime << "\n";
        p = p->next;
    }
}

// ========================
// Admin: View all payments
// ========================
void PaymentList::adminViewAllPayments() {
    Payment* p = head;
    cout << "--- ALL PAYMENTS ---\n";
    while (p) {
        cout << "[ID: " << p->paymentID << "] "
             << "Rider " << p->riderID
             << " -> Driver " << p->driverID
             << " | Amount: " << p->amount
             << " | " << p->datetime << "\n";
        p = p->next;
    }
}
