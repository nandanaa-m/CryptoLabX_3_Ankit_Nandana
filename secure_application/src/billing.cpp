#include "billing.h"

#include <iostream>
#include <string>

using namespace std;

void createBill(sqlite3* db)
{
    int patientId;
    double amount;
    string description;
    string date;

    cout << "\n--- Create Bill ---\n";

    cout << "Enter Patient ID: ";
    cin >> patientId;

    cout << "Enter Amount: ";
    cin >> amount;

    cin.ignore();

    cout << "Enter Description: ";
    getline(cin, description);

    cout << "Enter Date: ";
    getline(cin, date);

    string sql =
        "INSERT INTO bills "
        "(patient_id, amount, description, bill_date) "
        "VALUES (" +
        to_string(patientId) + ", " +
        to_string(amount) + ", '" +
        description + "', '" +
        date + "')";

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
        cout << "Failed to create bill.\n";

        if (errorMessage)
        {
            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }

        return;
    }

    cout << "Bill created successfully.\n";
}


static int billingCallback(
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


void viewBilling(sqlite3* db)
{
    int patientId;

    cout << "\n--- View Billing ---\n";

    cout << "Enter Patient ID: ";
    cin >> patientId;

    /*
        INTENTIONALLY VULNERABLE

        The application does NOT verify that the
        logged-in user owns this Patient ID.

        Therefore a user can access another patient's
        sensitive billing information.
    */

    string sql =
        "SELECT * FROM bills WHERE patient_id = " +
        to_string(patientId);

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        db,
        sql.c_str(),
        billingCallback,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK)
    {
        cout << "Failed to retrieve billing information.\n";

        if (errorMessage)
        {
            cout << "Database error: "
                 << errorMessage << endl;

            sqlite3_free(errorMessage);
        }
    }
}
