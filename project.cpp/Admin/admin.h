#ifndef ADMIN_H
#define ADMIN_H

#include <string>
using namespace std;

class Admin {
private:
    string adminUser = "IBRAHIM";
    string adminPass = "OBVIOUS";

public:
    bool login();

    void viewAllRiders();
    void viewAllDrivers();
    void viewAllVehicles();
    void viewAllRides();
};

#endif
