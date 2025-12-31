#include "Rider.h"      // Rider structure aur RiderList class ko include karne ke liye
#include <iostream>     
#include <fstream>      // file handling (ifstream, ofstream) ke liye
#include <sstream>      // stringstream use karne ke liye
#include <iomanip>      // setprecision aur fixed formatting ke liye
using namespace std;

// Constructor
RiderList::RiderList() { 
    head = nullptr;     // start mein linked list khali hoti hai is liye head null rakha
}

// Destructor
RiderList::~RiderList() {
    Rider* temp;        // temporary pointer banaya
    while(head) {       // jab tak list khali na ho
        temp = head;    // temp ko head pe rakha
        head = head->next; // head ko next rider pe move kar diya
        delete temp;    // purane rider ki memory free kar di
    }
}

// Generate unique ID
int RiderList::generateID() {
    int maxID = 0;              // sab se bari ID track karne ke liye
    Rider* temp = head;         // list traverse karne ke liye temp pointer
    while(temp) {               // poori list traverse hogi
        if(temp->id > maxID)    // agar current id bari ho
            maxID = temp->id;   // to maxID update kar do
        temp = temp->next;      // next rider pe move
    }
    return maxID + 1;           // nayi unique ID return
}

// Load riders from file
void RiderList::loadFromFile(const string& filename) {
    ifstream fin(filename);     // file open ki reading ke liye
    if(!fin) return;            // agar file na khule to function exit

    string line;
    while(getline(fin, line)) { // har line read karte jao
        stringstream ss(line);  // line ko stream mein convert kiya
        Rider* newRider = new Rider; // naya rider dynamically banaya
        string walletStr;

        getline(ss, line, ','); 
        newRider->id = stoi(line);   // ID string se int mein convert
        getline(ss, newRider->name, ','); // name read
        getline(ss, newRider->phone, ','); // phone read
        getline(ss, newRider->password, ','); // password read
        getline(ss, walletStr);       // wallet amount read
        newRider->wallet = stod(walletStr); // string se double mein convert

        newRider->next = head;  // naya rider list ke start mein add
        head = newRider;        // head ko update kiya
    }
    fin.close();                // file close
}

// Save riders to file
void RiderList::saveToFile(const string& filename) {
    ofstream fout(filename);    // file open writing ke liye
    Rider* temp = head;         // list traverse karne ke liye
    while(temp) {               // jab tak list end na ho
        fout << temp->id << "," 
             << temp->name << "," 
             << temp->phone << "," 
             << temp->password << "," 
             << fixed << setprecision(2) << temp->wallet << "\n"; // data comma separated format mein save
        temp = temp->next;      // next rider pe move
    }
    fout.close();               // file close
}

// Register Rider
void RiderList::registerRider() {
    Rider* newRider = new Rider;    // naya rider dynamically banaya
    newRider->id = generateID();    // unique ID generate ki

    cout << "Enter Name: "; 
    cin.ignore();                   // buffer clear kiya
    getline(cin, newRider->name);   // full name input liya

    cout << "Enter Phone: "; 
    cin >> newRider->phone;        

    cout << "Enter Password: "; 
    cin >> newRider->password;      // password input

    newRider->wallet = 1000.0;      // default wallet amount set ki
    newRider->next = head;          // list ke start mein insert
    head = newRider;                // head update

    saveToFile("Rider/riders.txt"); // updated data file mein save
    cout << "Registration successful! Your Rider ID is: " 
         << newRider->id << "\n";   // confirmation message
}

// Login Rider
bool RiderList::loginRider(int &riderID) {
    string phone, password;

    cout << "Enter Phone: "; 
    cin >> phone;                   // phone input

    cout << "Enter Password: "; 
    cin >> password;                // password input

    Rider* temp = head;             // list traversal start
    while(temp) {
        if(temp->phone == phone && temp->password == password) {
            cout << "Login successful! Welcome, " 
                 << temp->name << "\n"; // login success
            riderID = temp->id;     // logged-in rider ki ID store
            return true;            // login successful
        }
        temp = temp->next;          // next rider
    }
    cout << "Login failed! Invalid credentials.\n"; // agar match na mile
    return false;
}

// View Rider Profile
void RiderList::viewRiderProfile(int riderID) {
    Rider* temp = head;             // list traversal
    while(temp) {
        if(temp->id == riderID) {   // agar ID match ho
            cout << "------ Rider Profile ------\n";
            cout << "ID: " << temp->id << "\n";
            cout << "Name: " << temp->name << "\n";
            cout << "Phone: " << temp->phone << "\n";
            cout << "Wallet: " 
                 << fixed << setprecision(2) << temp->wallet << "\n";
            return;                 // profile show kar ke return
        }
        temp = temp->next;          // next rider
    }
    cout << "Rider not found!\n";   // agar ID exist na kare
}

// Add money to wallet
void RiderList::addWallet(int riderID, double amount) {
    Rider* temp = head;             // traversal start
    while(temp) {
        if(temp->id == riderID) {   // rider mil gaya
            temp->wallet += amount; // wallet update
            cout << "Wallet updated! New balance: " 
                 << fixed << setprecision(2) << temp->wallet << "\n";
            saveToFile("Rider/riders.txt"); // changes save
            return;
        }
        temp = temp->next;
    }
    cout << "Rider not found!\n";
}

// Helper function
Rider* RiderList::getRider(int riderID) {
    Rider* temp = head;             // traversal ke liye
    while(temp) {
        if(temp->id == riderID)     // agar ID match ho
            return temp;            // rider ka pointer return
        temp = temp->next;
    }
    return nullptr;                 // agar na mile
}

// DELETE rider
void RiderList::deleteRider(int riderID) {
    if (!head) {                    // agar list khali ho
        cout << "No riders found!\n";
        return;
    }

    Rider* temp = head;             // current rider
    Rider* prev = nullptr;          // previous rider

    while (temp) {
        if (temp->id == riderID) {  // rider mil gaya
            if (prev == nullptr) {  // agar head delete ho raha ho
                head = temp->next;
            } else {
                prev->next = temp->next; // previous ko next se link kar do
            }
            delete temp;          
            saveToFile("Rider/riders.txt"); // file update
            cout << "Rider deleted successfully!\n";
            return;
        }
        prev = temp;                // prev update
        temp = temp->next;          // next rider
    }

    cout << "Rider not found!\n";   // agar ID na mile
}

// PRINT ALL RIDERS (For Admin Panel)
void RiderList::printAll() { 
    if (!head) {                    // agar list empty ho
        cout << "No riders available!\n";
        return;
    }

    Rider* temp = head;             // traversal start
    cout << "\n===== ALL REGISTERED RIDERS =====\n";

    while (temp) {
        cout << "ID: " << temp->id
             << " | Name: " << temp->name
             << " | Phone: " << temp->phone
             << " | Wallet: " 
             << fixed << setprecision(2) << temp->wallet
             << "\n";
        temp = temp->next;          // next rider
    }}