#ifndef SECURITY_H
#define SECURITY_H

#include <string>
using namespace std;

class Security {
private:
    int lastOTP;                // stores last generated OTP
    int failedAttempts;         // for brute-force protection
    bool locked;                // lock account after 3 fails

public:
    Security();

    // OTP System
    int generateOTP();          // returns new OTP
    bool verifyOTP(int otp);    // check if OTP is correct

    // SOS System
    void sendSOS(const string& riderName, const string& location);

    // Failed login protection
    bool loginAttempt(bool success); // returns true if account locked

    // For debugging / admin
    bool isLocked();
    int getFailedAttempts();
};

#endif
