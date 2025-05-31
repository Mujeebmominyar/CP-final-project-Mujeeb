#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "unit.h"
#include "utils.h"

FILE *log_file = NULL;

void init_log() {
    // Create a log file with a timestamped filename
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    
    char filename[64];
    strftime(filename, sizeof(filename), "simulation_log_%Y%m%d_%H%M%S.txt", local_time);
    
    log_file = fopen(filename, "w");
    if (!log_file) {
        printf("Warning: Failed to create log file %s\n", filename);
        return;
    }
    
    printf("Log file created: %s\n", filename);
    
    // Write header to log file
    fprintf(log_file, "Emergency Dispatch Simulation Log\n");
    fprintf(log_file, "==================================\n");
    fprintf(log_file, "Started at: ");
    
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time);
    fprintf(log_file, "%s\n\n", time_str);
    
    fflush(log_file);
}

void log_simulation_start() {
    if (!log_file) return;
    fprintf(log_file, "SIMULATION STARTED\n");
    fflush(log_file);
}

void log_simulation_end() {
    if (!log_file) return;
    fprintf(log_file, "SIMULATION ENDED\n");
    fflush(log_file);
}

void log_incident_created(IncidentType type, Priority priority, int x, int y) {
    if (!log_file) return;
    fprintf(log_file, "New Incident Created: Type=%s, Priority=%s, Location=(%d,%d)\n", 
            getIncidentTypeStr(type), getPriorityStr(priority), x, y);
    fflush(log_file);
}

void log_unit_dispatched(Unit *u, int target_x, int target_y) {
    if (!log_file) return;
    fprintf(log_file, "Unit %d-%d dispatched to (%d,%d)\n",
            u->departmentNumber, u->unitNumber, target_x, target_y);
    fflush(log_file);
}

void log_incident_finished(IncidentType type, int x, int y) {
    if (!log_file) return;
    fprintf(log_file, "Incident Finished: Type=%s at Location=(%d,%d)\n",
            getIncidentTypeStr(type), x, y);
    fflush(log_file);
}

void log_unit_state_change(Unit *u, UnitState state) {
    if (!log_file) return;
    fprintf(log_file, "Unit %d-%d changed state to %s\n",
            u->departmentNumber, u->unitNumber, getUnitStateStr(state));
    fflush(log_file);
}

void log_unit_arrived(Unit *u, int incident_x, int incident_y) {
    if (!log_file) return;
    fprintf(log_file, "Unit %d-%d arrived at Incident (%d,%d)\n",
            u->departmentNumber, u->unitNumber, incident_x, incident_y);
    fflush(log_file);
}

void log_unit_returning(Unit *u, int base_x, int base_y) {
    if (!log_file) return;
    fprintf(log_file, "Unit %d-%d returning to base (%d,%d)\n",
            u->departmentNumber, u->unitNumber, base_x, base_y);
    fflush(log_file);
}

void log_incident_status_change(int x, int y, IncidentState state, IncidentType type) {
    if (!log_file) return;
    fprintf(log_file, "Incident %s at (%d,%d) changed state to %s\n", 
            getIncidentTypeStr(type), x, y, getIncidentStateStr(state));
    fflush(log_file);
}
