#include "map.h"
#include "common.h"
#include "incident.h"
#include "unit.h"
#include <stdio.h>
#include <string.h>

MapCell map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];
int map_width, map_height;

void clear_map() {
    // Clear the entire map grid
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            strcpy(map[y][x].symbol, " .  ");
        }
    }
}

void render_map() {
    // Print column numbers header
    printf("    ");
    for (int x = 0; x < map_width; x++) {
        printf("%2d  ", x);
    }
    printf("\n");
    
    // Print top border
    printf("   +");
    for (int x = 0; x < map_width; x++) {
        printf("----");
    }
    printf("+\n");
    
    // Print each row with row numbers and borders
    for (int y = 0; y < map_height; y++) {
        printf("%2d |", y);
        for (int x = 0; x < map_width; x++) {
            printf("%s", map[y][x].symbol);
        }
        printf("|\n");
    }
    
    // Print bottom border
    printf("   +");
    for (int x = 0; x < map_width; x++) {
        printf("----");
    }
    printf("+\n");
}

void place_departments() {
    for (int i = 0; i < department_count; i++) {
        char type_char = (departments[i].type == FIRE) ? 'F' :
                         (departments[i].type == MEDICAL) ? 'M' : 'P';
        sprintf(map[departments[i].y][departments[i].x].symbol, "_%c%d_", type_char, departments[i].number);
    }
}

void place_incidents() {
    // Place incidents on the map (only active ones)
    for (int i = 0; i < incident_count; i++) {
        if (incidents[i].state != INCIDENT_FINISHED) {
            char priority_char = (incidents[i].priority == LOW) ? 'L' :
                                (incidents[i].priority == MEDIUM) ? 'M' : 'H';
            char type_char = (incidents[i].type == FIRE) ? 'F' :
                            (incidents[i].type == MEDICAL) ? 'M' : 'P';
            
            sprintf(map[incidents[i].y][incidents[i].x].symbol, "%c%c* ", type_char, priority_char);
        }
    }
}

void place_units() {
    for (int i = 0; i < department_count; i++) {
        for (int j = 0; j < departments[i].unit_count; j++) {
            Unit *u = &departments[i].units[j];
            
            // Only place unit if the cell is empty (no incident or department)
            if (strcmp(map[u->y][u->x].symbol, " .  ") == 0) {
                char type_char = (u->type == FIRE) ? 'F' :
                                 (u->type == MEDICAL) ? 'M' : 'P';
                sprintf(map[u->y][u->x].symbol, "%c%d-%d", 
                        type_char, u->departmentNumber, u->unitNumber);
            }
        }
    }
}

void update_and_render() {
    // Clear the map first
    clear_map();
    
    // Place elements in order of priority (lowest to highest priority)
    // 1. Place departments first (lowest priority for rendering)
    place_departments();
    
    // 2. Place units (medium priority)
    place_units();
    
    // 3. Place incidents last (highest priority - they should be visible)
    place_incidents();
    
    // Finally, render the complete map
    render_map();
}
