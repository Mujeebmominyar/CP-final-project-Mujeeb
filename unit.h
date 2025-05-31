#ifndef UNIT_H
#define UNIT_H

#include "common.h"

typedef struct {
    int x, y;
    int target_x, target_y;
    IncidentType type;
    int departmentNumber;
    int unitNumber;
    UnitState state;
} Unit;

typedef struct {
    int x, y;
    IncidentType type;
    int number;
    Unit units[MAX_UNITS_PER_DEPARTMENT];
    int unit_count;
} Department;

extern Department departments[MAX_DEPARTMENTS];
extern int department_count;

// Function declarations
int manhattan_distance(int x1, int y1, int x2, int y2);
void move_unit_one_step(Unit *u);
void update_units_movement();

#endif // UNIT_H
