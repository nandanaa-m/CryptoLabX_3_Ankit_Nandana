#include <iostream>

#include "database.h"
#include "appointments.h"
#include "prescriptions.h"
#include "billing.h"
#include "records.h"

using namespace std;

int main()
{
    sqlite3* db = openDatabase("hospital.db");

    if (!db)
    {
        return 1;
    }

    initializeDatabase(db);

    int choice;

    do
    {
        cout << "\n====================================\n";
        cout << "     HOSPITAL MANAGEMENT SYSTEM\n";
        cout << "====================================\n";

        cout << "1. Schedule Appointment\n";
        cout << "2. Search Appointment\n";
        cout << "3. Add Prescription\n";
        cout << "4. View Prescription\n";
        cout << "5. Create Bill\n";
        cout << "6. View Billing\n";
        cout << "7. Register Patient\n";
        cout << "8. View Medical Record\n";
        cout << "9. Exit\n";

        cout << "\nEnter choice: ";

if (!(cin >> choice))
{
    cout << "Invalid input. Please enter a number from 1 to 9.\n";

    cin.clear();
    cin.ignore(10000, '\n');

    continue;
}

        switch (choice)
        {
            case 1:
                scheduleAppointment(db);
                break;

            case 2:
                lookupAppointment(db);
                break;

            case 3:
                addPrescription(db);
                break;

            case 4:
                viewPrescription(db);
                break;

            case 5:
                createBill(db);
                break;

            case 6:
                viewBilling(db);
                break;

            case 7:
                registerPatient();
                break;

            case 8:
                viewMedicalRecord();
                break;

            case 9:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 7);

    closeDatabase(db);

    return 0;
}
