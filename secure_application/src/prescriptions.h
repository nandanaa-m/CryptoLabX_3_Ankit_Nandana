#ifndef PRESCRIPTIONS_H
#define PRESCRIPTIONS_H

#include <sqlite3.h>

void addPrescription(sqlite3* db);
void viewPrescription(sqlite3* db);

#endif
