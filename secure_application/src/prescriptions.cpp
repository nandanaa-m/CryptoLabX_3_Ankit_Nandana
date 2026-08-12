#include "prescriptions.h"

#include <iostream>
#include <string>

using namespace std;

void addPrescription(sqlite3* db)
{
    int patientId;
    string doctor;
    string medicine;
    string dosage;
    string instructions;

    cout << "\n--- Add Prescription ---\n";

    cout << "Enter Patient ID: ";
    cin >> patientId;

    cin.ignore();

    cout << "Enter Doctor Name: ";
    getline(cin, doctor);

    cout << "Enter Medicine: ";
    getline(cin, medicine);

    cout << "Enter Dosage: ";
    getline(cin, dosage);

    cout << "Enter Instructions: ";
    getline(cin, instructions);

    string sql =
        "INSERT INTO prescriptions "
        "(patient_id, doctor, medicine, dosage, instructions) "
        "VALUES (" +
        to_string(patientId) + ", '" +
        doctor + "', '" +
        medicine + "', '" +
        dosage + "', '" +
        instructions + "')";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK)
    {
        cout << "Failed to add prescription.\n";

        if (errorMessage)
        {
            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }

        return;
    }

    cout << "Prescription added successfully.\n";
}


static int prescriptionCallback(
    void*,
    int columnCount,
    char** values,
    char** columnNames)
{
    for (int i = 0; i < columnCount; i++)
    {
        cout << columnNames[i]
             << ": "
             << (values[i] ? values[i] : "NULL")
             << endl;
    }

    cout << "-------------------------\n";

    return 0;
}


void viewPrescription(sqlite3* db)
{
    int patientId;

    cout << "\n--- View Prescription ---\n";

    cout << "Enter Patient ID: ";
    cin >> patientId;

    /*
        INTENTIONALLY VULNERABLE

        No authorization check is performed.

        Any user can enter another patient's ID
        and access prescription information.
    */

    string sql =
        "SELECT * FROM prescriptions WHERE patient_id = " +
        to_string(patientId);

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        db,
        sql.c_str(),
        prescriptionCallback,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK)
    {
        cout << "Failed to retrieve prescription.\n";

        if (errorMessage)
        {
            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }
    }
}
