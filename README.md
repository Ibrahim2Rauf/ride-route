# 🎯 Smart Riding System

**Smart Riding System** is a C++-based project developed as my **3rd semester project**.  
It is designed to enhance rider **safety, convenience, and efficiency** using **OTP-based security**, real-time tracking, and automated alerts.

---

## ✅ Features

### 🔐 Authentication & Security
- ✔ Secure login with OTP verification  
- ✔ User accounts stored securely (`users.txt`)  
- ✔ Maximum 3 login attempts per user  

### 🏍 Ride Management System
- ✔ Add, update, and delete rides  
- ✔ Track ride status and statistics  
- ✔ Efficient ride data handling  

### ⚠️ Alerts & Notifications
- ✔ Safety alerts for speed or location issues  
- ✔ Notifications for ride events and updates  

### 📋 Audit & Logging
- ✔ Logs login, ride events, and alerts  
- ✔ File: `audit.txt` (append-only)  
- ✔ Monitors system activity for review  

---

## 🗂 Project Structure
SmartRidingSystem/
├── main.cpp # Main program file
├── classes/ # Contains all class files
├── data/ # Sample txt files for users, rides, and audit logs
└── README.md # Project documentation

---

## ⚙️ Technologies Used
- C++ (Object-Oriented Programming, Classes, Inheritance)  
- File Handling (text + binary)  
- Console-based interface  
- OTP simulation for security  

---

## 💻 How to Compile & Run
```bash
g++ main.cpp classes/*.cpp -o smart_riding_system
./smart_riding_system
