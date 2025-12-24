#ifndef ADMIN_H
#define ADMIN_H

#include <string>
using namespace std;

// Forward declarations
class RiderList;
class DriverList;

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

   
    void deleteRider(int riderID, RiderList &riders);
    void deleteDriver(int driverID, DriverList &drivers);
};

#endif
