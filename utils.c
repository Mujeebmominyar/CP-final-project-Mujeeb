#include "utils.h"

const char* getPriorityStr(Priority p) {
    switch (p) {
        case LOW: return "LOW";
        case MEDIUM: return "MEDIUM";
        case HIGH: return "HIGH";
        default: return "UNKNOWN";
    }
}

const char* getIncidentTypeStr(IncidentType t) {
    switch (t) {
        case FIRE: return "FIRE";
        case MEDICAL: return "MEDICAL";
        case POLICE: return "POLICE";
        default: return "UNKNOWN";
    }
}

const char* getUnitTypeStr(IncidentType t) {
    return getIncidentTypeStr(t);
}

const char* getUnitStateStr(UnitState s) {
    switch (s) {
        case UNIT_WAITING: return "WAITING";
        case UNIT_DISPATCHED: return "DISPATCHED";
        case UNIT_OPERATING: return "OPERATING";
        case UNIT_RETURNING: return "RETURNING";
        default: return "UNKNOWN";
    }
}

const char* getIncidentStateStr(IncidentState s) {
    switch (s) {
        case INCIDENT_WAITING: return "WAITING";
        case INCIDENT_OPERATION: return "OPERATION";
        case INCIDENT_FINISHED: return "FINISHED";
        default: return "UNKNOWN";
    }
}
