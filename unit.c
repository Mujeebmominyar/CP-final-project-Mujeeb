#include "unit.h"
#include "map.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>

Department departments[MAX_DEPARTMENTS];
int department_count = 0;

int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void move_unit_one_step(Unit *u) {
    // Move unit one step toward its target
    // Prioritize X direction first
    
    if (u->x != u->target_x) {
        if (u->x < u->target_x) {
            u->x++;
        } else {
            u->x--;
        }
    } else if (u->y != u->target_y) {
        if (u->y < u->target_y) {
            u->y++;
        } else {
            u->y--;
        }
    }
    
    // Check if unit has reached its target
    if (u->x == u->target_x && u->y == u->target_y) {
        if (u->state == UNIT_DISPATCHED) {
            // Unit arrived at incident
            log_unit_arrived(u, u->target_x, u->target_y);
            // Note: State change to OPERATING will be handled in update_incidents()
        } else if (u->state == UNIT_RETURNING) {
            // Unit returned to base
            u->state = UNIT_WAITING;
            log_unit_state_change(u, UNIT_WAITING);
        }
    }
}

void update_units_movement() {
    // Iterate through all departments and units
    for (int i = 0; i < department_count; i++) {
        for (int j = 0; j < departments[i].unit_count; j++) {
            Unit *u = &departments[i].units[j];
            
            // Move units that are dispatched or returning
            if (u->state == UNIT_DISPATCHED || u->state == UNIT_RETURNING) {
                move_unit_one_step(u);
            }
        }
    }
}
