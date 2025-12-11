#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
using namespace std;

class RiderList;
class DriverList;

struct Payment {
    int paymentID;
    int riderID;
    int driverID;
    double amount;
    string datetime;
    Payment* next;
};

class PaymentList {
private:
    Payment* head;

public:
    PaymentList();
    ~PaymentList();

    int generateID();
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);

    void makePayment(int riderID, int driverID, double amount,
                     RiderList &riders, DriverList &drivers);

    void viewRiderPayments(int riderID);
    void viewDriverPayments(int driverID);
    void adminViewAllPayments();
};

#endif
