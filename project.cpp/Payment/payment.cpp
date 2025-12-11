#include "Payment.h"
#include "../Rider/Rider.h"
#include "../Driver/Driver.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;

PaymentList::PaymentList() {
    head = nullptr;
}

PaymentList::~PaymentList() {
    Payment* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

int PaymentList::generateID() {
    int mx = 0;
    Payment* p = head;
    while (p) {
        if (p->paymentID > mx) mx = p->paymentID;
        p = p->next;
    }
    return mx + 1;
}

void PaymentList::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin) return;

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        Payment* p = new Payment;

        getline(ss, line, ','); p->paymentID = stoi(line);
        getline(ss, line, ','); p->riderID = stoi(line);
        getline(ss, line, ','); p->driverID = stoi(line);
        getline(ss, line, ','); p->amount = stod(line);
        getline(ss, p->datetime);

        p->next = head;
        head = p;
    }

    fin.close();
}

void PaymentList::saveToFile(const string& filename) {
    ofstream fout(filename);

    Payment* p = head;
    while (p) {
        fout << p->paymentID << ","
             << p->riderID << ","
             << p->driverID << ","
             << fixed << setprecision(2) << p->amount << ","
             << p->datetime << "\n";
        p = p->next;
    }

    fout.close();
}

void PaymentList::makePayment(int riderID, int driverID, double amount,
                              RiderList &riders, DriverList &drivers) {

    Rider* r = riders.getRider(riderID);
    Driver* d = drivers.getDriver(driverID);

    if (!r || !d) {
        cout << "Error: Invalid rider or driver!\n";
        return;
    }

    if (r->wallet < amount) {
        cout << "Payment failed! Rider does not have enough balance.\n";
        return;
    }

    // money transfer
    r->wallet -= amount;
    d->wallet += amount;

    riders.saveToFile("Rider/riders.txt");
    drivers.saveToFile("Driver/drivers.txt");

    // create record
    Payment* p = new Payment;
    p->paymentID = generateID();
    p->riderID = riderID;
    p->driverID = driverID;
    p->amount = amount;

    // time stamp
    time_t now = time(0);
    p->datetime = ctime(&now);
    p->datetime.pop_back(); // remove \n

    p->next = head;
    head = p;

    saveToFile("Payment/payments.txt");

    cout << "Payment successful!\n";
}

void PaymentList::viewRiderPayments(int riderID) {
    Payment* p = head;
    cout << "--- Rider Payment History ---\n";
    while (p) {
        if (p->riderID == riderID)
            cout << "Paid: " << p->amount << " to Driver " << p->driverID
                 << " | On: " << p->datetime << "\n";
        p = p->next;
    }
}

void PaymentList::viewDriverPayments(int driverID) {
    Payment* p = head;
    cout << "--- Driver Payment History ---\n";
    while (p) {
        if (p->driverID == driverID)
            cout << "Received: " << p->amount << " from Rider " << p->riderID
                 << " | On: " << p->datetime << "\n";
        p = p->next;
    }
}

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
