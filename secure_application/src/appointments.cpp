#include "appointments.h"

#include <iostream>
#include <string>

using namespace std;

void scheduleAppointment(sqlite3* db)
{
    string patientId;
    string doctor;
    string date;
    string time;

    cout << "\n--- Schedule Appointment ---\n";

    cout << "Enter Patient ID: ";
    cin >> patientId;

    cin.ignore();

    cout << "Enter Doctor Name: ";
    getline(cin, doctor);

    cout << "Enter Date: ";
    getline(cin, date);

    cout << "Enter Time: ";
    getline(cin, time);

    string sql =
        "INSERT INTO appointments "
        "(patient_id, doctor, appointment_date, appointment_time) "
        "VALUES ('" +
        patientId + "', '" +
        doctor + "', '" +
        date + "', '" +
        time + "')";

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
        cout << "Failed to create appointment.\n";

        if (errorMessage)
        {
            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }

        return;
    }

    cout << "Appointment created successfully.\n";
}


static int appointmentCallback(
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


void lookupAppointment(sqlite3* db)
{
    string patientId;

    cout << "\n--- Appointment Lookup ---\n";

    cout << "Enter Patient ID: ";
    cin >> patientId;

    /*
        INTENTIONALLY VULNERABLE CODE

        The user input is directly concatenated
        into the SQL query.

        This is SQL Injection vulnerability.
    */

    string query =
        "SELECT * FROM appointments WHERE patient_id = '" +
        patientId + "'";

    cout << "\nExecuting appointment lookup...\n";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        db,
        query.c_str(),
        appointmentCallback,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK)
    {
        cout << "Query failed.\n";

        if (errorMessage)
        {
            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }

        return;
    }
}
