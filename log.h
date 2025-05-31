#ifndef LOG_H
#define LOG_H

#include "common.h"
#include "unit.h"
#include "stdio.h"

extern FILE *log_file;

void init_log();
void log_simulation_start();
void log_simulation_end();
void log_incident_created(IncidentType type, Priority priority, int x, int y);
void log_unit_dispatched(Unit *u, int target_x, int target_y);
void log_incident_finished(IncidentType type, int x, int y);
void log_unit_state_change(Unit *u, UnitState state);
void log_unit_arrived(Unit *u, int incident_x, int incident_y);
void log_unit_returning(Unit *u, int base_x, int base_y);
void log_incident_status_change(int x, int y, IncidentState state, IncidentType type);

#endif // LOG_H
