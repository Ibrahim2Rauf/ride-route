#ifndef SECURITY_H
#define SECURITY_H

#include <string>
using namespace std;

class Security {
private:
    int lastOTP;               
    int failedAttempts;        
    bool locked;                

public:
    Security();

    // OTP System
    int generateOTP();        
    bool verifyOTP(int otp);    

    // SOS System
    void sendSOS(const string& riderName, const string& location);

    // Failed login protection
    bool loginAttempt(bool success); 

    // For debugging / admin
    bool isLocked();
    int getFailedAttempts();
};

#endif
