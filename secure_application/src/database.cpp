#include "database.h"
#include <iostream>

sqlite3* openDatabase(const std::string& filename)
{
    sqlite3* db = nullptr;

    int result = sqlite3_open(filename.c_str(), &db);

    if (result != SQLITE_OK)
    {
        std::cerr << "Database opening failed: "
                  << sqlite3_errmsg(db) << std::endl;

        if (db)
            sqlite3_close(db);

        return nullptr;
    }

    return db;
}

void initializeDatabase(sqlite3* db)
{
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS appointments (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            patient_id INTEGER NOT NULL,
            doctor TEXT NOT NULL,
            appointment_date TEXT NOT NULL,
            appointment_time TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS prescriptions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            patient_id INTEGER NOT NULL,
            doctor TEXT NOT NULL,
            medicine TEXT NOT NULL,
            dosage TEXT NOT NULL,
            instructions TEXT
        );

        CREATE TABLE IF NOT EXISTS bills (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            patient_id INTEGER NOT NULL,
            amount REAL NOT NULL,
            description TEXT NOT NULL,
            bill_date TEXT NOT NULL
        );
    )";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        db,
        sql,
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK)
    {
        std::cerr << "Database initialization failed: "
                  << errorMessage << std::endl;

        sqlite3_free(errorMessage);
    }
}

void closeDatabase(sqlite3* db)
{
    if (db)
        sqlite3_close(db);
}
