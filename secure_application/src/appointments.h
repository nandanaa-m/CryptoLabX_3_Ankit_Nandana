#ifndef APPOINTMENTS_H
#define APPOINTMENTS_H

#include "sqlite3.h"

void scheduleAppointment(sqlite3* db);
void lookupAppointment(sqlite3* db);

#endif
