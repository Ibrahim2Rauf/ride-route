#include "Rider.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

// Constructor
RiderList::RiderList() {
    head = nullptr;
}

// Destructor
RiderList::~RiderList() {
    Rider* temp;
    while(head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

// Generate unique ID
int RiderList::generateID() {
    int maxID = 0;
    Rider* temp = head;
    while(temp) {
        if(temp->id > maxID)
            maxID = temp->id;
        temp = temp->next;
    }
    return maxID + 1;
}

// Load riders from file
void RiderList::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if(!fin) return; // file may not exist

    string line;
    while(getline(fin, line)) {
        stringstream ss(line);
        Rider* newRider = new Rider;
        string walletStr;
        getline(ss, line, ','); newRider->id = stoi(line);
        getline(ss, newRider->name, ',');
        getline(ss, newRider->phone, ',');
        getline(ss, newRider->password, ',');
        getline(ss, walletStr);
        newRider->wallet = stod(walletStr);
        newRider->next = head;
        head = newRider;
    }
    fin.close();
}

// Save riders to file
void RiderList::saveToFile(const string& filename) {
    ofstream fout(filename);
    Rider* temp = head;
    while(temp) {
        fout << temp->id << "," 
             << temp->name << "," 
             << temp->phone << "," 
             << temp->password << "," 
             << fixed << setprecision(2) << temp->wallet << "\n";
        temp = temp->next;
    }
    fout.close();
}

// Register Rider
void RiderList::registerRider() {
    Rider* newRider = new Rider;
    newRider->id = generateID();
    cout << "Enter Name: "; cin.ignore(); getline(cin, newRider->name);
    cout << "Enter Phone: "; cin >> newRider->phone;
    cout << "Enter Password: "; cin >> newRider->password;
    newRider->wallet = 1000; // default wallet balance
    newRider->next = head;
    head = newRider;

    saveToFile("riders.txt");
    cout << "Registration successful! Your Rider ID is: " << newRider->id << "\n";
    system("pause");
}

// Login Rider
bool RiderList::loginRider(int &riderID) {
    string phone, password;
    cout << "Enter Phone: "; cin >> phone;
    cout << "Enter Password: "; cin >> password;

    Rider* temp = head;
    while(temp) {
        if(temp->phone == phone && temp->password == password) {
            cout << "Login successful! Welcome, " << temp->name << "\n";
            riderID = temp->id;
            system("pause");
            return true;
        }
        temp = temp->next;
    }
    cout << "Login failed! Invalid credentials.\n";
    system("pause");
    return false;
}

// View Rider Profile
void RiderList::viewRiderProfile(int riderID) {
    Rider* temp = head;
    while(temp) {
        if(temp->id == riderID) {
            cout << "------ Rider Profile ------\n";
            cout << "ID: " << temp->id << "\n";
            cout << "Name: " << temp->name << "\n";
            cout << "Phone: " << temp->phone << "\n";
            cout << "Wallet: " << fixed << setprecision(2) << temp->wallet << "\n";
            system("pause");
            return;
        }
        temp = temp->next;
    }
    cout << "Rider not found!\n";
    system("pause");
}

// Add money to wallet
void RiderList::addWallet(int riderID, double amount) {
    Rider* temp = head;
    while(temp) {
        if(temp->id == riderID) {
            temp->wallet += amount;
            cout << "Wallet updated! New balance: " << fixed << setprecision(2) << temp->wallet << "\n";
            saveToFile("riders.txt");
            system("pause");
            return;
        }
        temp = temp->next;
    }
    cout << "Rider not found!\n";
    system("pause");
}
