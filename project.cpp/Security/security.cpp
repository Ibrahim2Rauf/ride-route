#include "Security.h"
#include <iostream>
#include <cstdlib>   // rand()
#include <ctime>     // time()

using namespace std;

Security::Security() {
    lastOTP = -1;
    failedAttempts = 0;
    locked = false;
    srand(time(0));
}

/* ---------------- OTP SYSTEM ---------------- */

int Security::generateOTP() {
    lastOTP = rand() % 9000 + 1000;   // 1000–9999
    cout << "\n[SECURITY] Your OTP is: " << lastOTP << "\n";
    return lastOTP;
}

bool Security::verifyOTP(int otp) {
    if (locked) {
        cout << "\n[ERROR] Account locked due to multiple failed OTP attempts!\n";
        return false;
    }

    if (otp == lastOTP) {
        cout << "[SECURITY] OTP Verified Successfully.\n";
        failedAttempts = 0;
        return true;
    }

    failedAttempts++;
    cout << "[SECURITY] Incorrect OTP! (" << failedAttempts << "/3)\n";

    if (failedAttempts >= 3) {
        locked = true;
        cout << "[SECURITY] Account is now LOCKED!\n";
    }

    return false;
}

/* ---------------- SOS SYSTEM ---------------- */

void Security::sendSOS(const string& riderName, const string& location) {
    cout << "\n====== EMERGENCY SOS ALERT ======\n";
    cout << "Rider: " << riderName << "\n";
    cout << "Location: " << location << "\n";
    cout << "Status: HELP REQUEST SENT to Admin + Nearby Drivers!\n";
    cout << "=================================\n";
}

/* ----------- LOGIN BRUTE-FORCE PROTECTION ----------- */

bool Security::loginAttempt(bool success) {
    if (success) {
        failedAttempts = 0;
        return false;
    }
    failedAttempts++;

    if (failedAttempts >= 3) {
        locked = true;
        cout << "[SECURITY] Too many failed attempts! Account LOCKED.\n";
        return true;
    }

    return false;
}

/* ------------- GETTERS ------------- */

bool Security::isLocked() { return locked; }
int Security::getFailedAttempts() { return failedAttempts; }
