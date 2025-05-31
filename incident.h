#ifndef INCIDENT_H
#define INCIDENT_H

#include "common.h"
#include "unit.h"
#include "stdlib.h"

typedef struct {
    int fire_units;
    int medical_units;
    int police_units;
    int time_required;
} IncidentRequirement;

typedef struct {
    int x, y;
    IncidentType type;
    Priority priority;
    IncidentState state;
    int operation_turns_remaining;
    Unit *dispatched_units[3][MAX_REQUIRED_UNITS_PER_TYPE];
    int dispatched_count[3];
} Incident;

extern Incident incidents[MAX_INCIDENTS];
extern int incident_count;
extern int incident_capacity;

// Incident requirements table [incident_type][priority]
extern IncidentRequirement incident_requirements[3][3];

// Function declarations
void dispatch_units();
void update_incidents();
void ensure_incident_capacity();

#endif // INCIDENT_H
