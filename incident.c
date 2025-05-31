#include "incident.h"
#include "log.h"
#include "unit.h"
#include "map.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

Incident incidents[MAX_INCIDENTS];
int incident_count = 0;
int incident_capacity = MAX_INCIDENTS;

// Incident requirements for different incident types and priorities
// Format: {fire_units, medical_units, police_units, time_required}
IncidentRequirement incident_requirements[3][3] = {
    // FIRE incidents
    {{1, 0, 0, 3}, {1, 1, 0, 6}, {1, 1, 1, 8}}, // LOW, MEDIUM, HIGH
    // MEDICAL incidents  
    {{0, 1, 0, 1}, {0, 1, 0, 3}, {0, 1, 0, 5}}, // LOW, MEDIUM, HIGH
    // POLICE incidents
    {{0, 0, 1, 2}, {0, 1, 1, 4}, {0, 1, 1, 8}}  // LOW, MEDIUM, HIGH
};

Unit* find_nearest_available_unit(IncidentType type, int incident_x, int incident_y) {
    Unit *nearest_unit = NULL;
    int min_distance = INT_MAX;
    
    for (int i = 0; i < department_count; i++) {
        if (departments[i].type == type) {
            for (int j = 0; j < departments[i].unit_count; j++) {
                Unit *unit = &departments[i].units[j];
                if (unit->state == UNIT_WAITING) {
                    int distance = manhattan_distance(unit->x, unit->y, incident_x, incident_y);
                    if (distance < min_distance) {
                        min_distance = distance;
                        nearest_unit = unit;
                    }
                }
            }
        }
    }
    
    return nearest_unit;
}

void dispatch_units() {
    for (int i = 0; i < incident_count; i++) {
        Incident *incident = &incidents[i];
        
        if (incident->state != INCIDENT_WAITING) {
            continue;
        }
        
        IncidentRequirement req = incident_requirements[incident->type][incident->priority];
        
        // Try to dispatch fire units
        while (incident->dispatched_count[FIRE] < req.fire_units) {
            Unit *unit = find_nearest_available_unit(FIRE, incident->x, incident->y);
            if (unit) {
                unit->state = UNIT_DISPATCHED;
                unit->target_x = incident->x;
                unit->target_y = incident->y;
                incident->dispatched_units[FIRE][incident->dispatched_count[FIRE]] = unit;
                incident->dispatched_count[FIRE]++;
                log_unit_dispatched(unit, incident->x, incident->y);
                log_unit_state_change(unit, UNIT_DISPATCHED);
            } else {
                break; // No available fire units
            }
        }
        
        // Try to dispatch medical units
        while (incident->dispatched_count[MEDICAL] < req.medical_units) {
            Unit *unit = find_nearest_available_unit(MEDICAL, incident->x, incident->y);
            if (unit) {
                unit->state = UNIT_DISPATCHED;
                unit->target_x = incident->x;
                unit->target_y = incident->y;
                incident->dispatched_units[MEDICAL][incident->dispatched_count[MEDICAL]] = unit;
                incident->dispatched_count[MEDICAL]++;
                log_unit_dispatched(unit, incident->x, incident->y);
                log_unit_state_change(unit, UNIT_DISPATCHED);
            } else {
                break; // No available medical units
            }
        }
        
        // Try to dispatch police units
        while (incident->dispatched_count[POLICE] < req.police_units) {
            Unit *unit = find_nearest_available_unit(POLICE, incident->x, incident->y);
            if (unit) {
                unit->state = UNIT_DISPATCHED;
                unit->target_x = incident->x;
                unit->target_y = incident->y;
                incident->dispatched_units[POLICE][incident->dispatched_count[POLICE]] = unit;
                incident->dispatched_count[POLICE]++;
                log_unit_dispatched(unit, incident->x, incident->y);
                log_unit_state_change(unit, UNIT_DISPATCHED);
            } else {
                break; // No available police units
            }
        }
    }
}

int check_all_units_arrived(Incident *incident) {
    IncidentRequirement req = incident_requirements[incident->type][incident->priority];
    
    // Check if we have enough units dispatched
    if (incident->dispatched_count[FIRE] < req.fire_units ||
        incident->dispatched_count[MEDICAL] < req.medical_units ||
        incident->dispatched_count[POLICE] < req.police_units) {
        return 0; // Not enough units dispatched yet
    }
    
    // Check if all dispatched units have arrived
    for (int type = 0; type < 3; type++) {
        for (int j = 0; j < incident->dispatched_count[type]; j++) {
            Unit *unit = incident->dispatched_units[type][j];
            if (unit && (unit->x != incident->x || unit->y != incident->y)) {
                return 0; // Unit hasn't arrived yet
            }
        }
    }
    
    return 1; // All required units have arrived
}

void update_incidents() {
    for (int i = 0; i < incident_count; i++) {
        Incident *incident = &incidents[i];
        
        if (incident->state == INCIDENT_WAITING) {
            // Check if all required units have arrived
            if (check_all_units_arrived(incident)) {
                incident->state = INCIDENT_OPERATION;
                log_incident_status_change(incident->x, incident->y, INCIDENT_OPERATION, incident->type);
                
                // Set all arrived units to OPERATING state
                for (int type = 0; type < 3; type++) {
                    for (int j = 0; j < incident->dispatched_count[type]; j++) {
                        Unit *unit = incident->dispatched_units[type][j];
                        if (unit) {
                            unit->state = UNIT_OPERATING;
                            log_unit_state_change(unit, UNIT_OPERATING);
                        }
                    }
                }
            }
        }
        else if (incident->state == INCIDENT_OPERATION) {
            incident->operation_turns_remaining--;
            
            if (incident->operation_turns_remaining <= 0) {
                incident->state = INCIDENT_FINISHED;
                log_incident_status_change(incident->x, incident->y, INCIDENT_FINISHED, incident->type);
                log_incident_finished(incident->type, incident->x, incident->y);
                
                // Set all units to return to base
                for (int type = 0; type < 3; type++) {
                    for (int j = 0; j < incident->dispatched_count[type]; j++) {
                        Unit *unit = incident->dispatched_units[type][j];
                        if (unit) {
                            unit->state = UNIT_RETURNING;
                            
                            // Find the unit's home department
                            for (int d = 0; d < department_count; d++) {
                                if (departments[d].type == unit->type && 
                                    departments[d].number == unit->departmentNumber) {
                                    unit->target_x = departments[d].x;
                                    unit->target_y = departments[d].y;
                                    log_unit_returning(unit, departments[d].x, departments[d].y);
                                    log_unit_state_change(unit, UNIT_RETURNING);
                                    break;
                                }
                            }
                        }
                    }
                }
                
                // Reset dispatch counts
                for (int type = 0; type < 3; type++) {
                    incident->dispatched_count[type] = 0;
                    for (int j = 0; j < MAX_REQUIRED_UNITS_PER_TYPE; j++) {
                        incident->dispatched_units[type][j] = NULL;
                    }
                }
            }
        }
    }
}

void ensure_incident_capacity() {
    if (incident_count >= incident_capacity) {
        printf("Warning: Maximum incident capacity reached (%d)\n", MAX_INCIDENTS);
        // In this static array implementation, we just warn the user
        // In a dynamic implementation, we would reallocate memory here
    }
}
