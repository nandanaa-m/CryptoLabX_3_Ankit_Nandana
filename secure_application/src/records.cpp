#include <bits/stdc++.h>
using namespace std;

// Function 1: Register a new patient and save to a text file
void registerPatient() {
    string id, name, age, diagnosis;
    
    cout << "\n--- Patient Registration ---\n";
    cout << "Enter Patient ID (e.g., 101): ";
    cin >> id;
    cin.ignore(); // Clears the newline character from the input buffer
    
    cout << "Enter Patient Name: ";
    getline(cin, name);
    
    cout << "Enter Age: ";
    cin >> age;
    cin.ignore();
    
    cout << "Enter Diagnosis: ";
    getline(cin, diagnosis);

    // Save into the testcases folder
    string filename = "testcases/patient_" + id + ".txt";
    ofstream outFile(filename);
    
    if (outFile.is_open()) {
        outFile << "Patient ID: " << id << "\n";
        outFile << "Name: " << name << "\n";
        outFile << "Age: " << age << "\n";
        outFile << "Diagnosis: " << diagnosis << "\n";
        outFile.close();
        cout << "[+] Patient successfully registered and saved to " << filename << "\n";
    } else {
        cout << "[!] Error: Could not save patient record. Ensure the 'testcases' folder exists.\n";
    }
}

// Function 2: View Medical Record (CONTAINS PATH TRAVERSAL VULNERABILITY)
void viewMedicalRecord() {
    string patientID;
    
    cout << "\n--- View Medical Record ---\n";
    cout << "Enter Patient ID: ";
    cin >> patientID;

    // VULNERABILITY: Path Traversal
    // Directly concatenating user input without sanitization. 
    // If a user types "../src/records.cpp", it will traverse outside the folder and read source code.
    string filepath = "testcases/patient_" + patientID + ".txt";
    
    ifstream file(filepath);
    
    if (file.is_open()) {
        string line;
        cout << "\n[File Contents of " << filepath << "]:\n";
        cout << "----------------------------------------\n";
        while (getline(file, line)) {
            cout << line << "\n";
        }
        cout << "----------------------------------------\n";
        file.close();
    } else {
        cout << "[!] Error: Record not found for path: " << filepath << "\n";
    }
}