#ifndef BILLING_H
#define BILLING_H

#include <sqlite3.h>

void createBill(sqlite3* db);
void viewBilling(sqlite3* db);

#endif
