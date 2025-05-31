#include "config.h"
#include "map.h"
#include "unit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_configuration(const char *filename) {
    FILE *file = fopen("sample_config.txt", "r");
    if (!file) {
        printf("Failed to open configuration file: %s\n", filename);
        exit(1);
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MAP_SIZE", 8) == 0) {
            sscanf(line, "MAP_SIZE %d %d", &map_width, &map_height);
        } 
        else if (strncmp(line, "DEPARTMENT", 10) == 0) {
            // Parse "DEPARTMENT" lines from the configuration file
            char type_str[16];
            int dept_number, x, y, unit_count;
            
            if (sscanf(line, "DEPARTMENT %s %d %d %d %d", 
                      type_str, &dept_number, &x, &y, &unit_count) == 5) {
                
                // Create new department
                Department *dept = &departments[department_count];
                dept->type = parse_department_type(type_str);
                dept->number = dept_number;
                dept->x = x;
                dept->y = y;
                dept->unit_count = unit_count;
                
                // Initialize units for this department
                for (int i = 0; i < unit_count; i++) {
                    Unit *unit = &dept->units[i];
                    unit->x = x;
                    unit->y = y;
                    unit->target_x = x;
                    unit->target_y = y;
                    unit->type = dept->type;
                    unit->departmentNumber = dept_number;
                    unit->unitNumber = i + 1;
                    unit->state = UNIT_WAITING;
                }
                
                department_count++;
                
                printf("Loaded department: %s-%d at (%d,%d) with %d units\n",
                       type_str, dept_number, x, y, unit_count);
            }
        }
    }

    fclose(file);
}

IncidentType parse_department_type(const char *str) {
    if (strcmp(str, "FIRE") == 0) return FIRE;
    if (strcmp(str, "MEDICAL") == 0) return MEDICAL;
    return POLICE;
}

Priority parse_priority(const char *str) {
    if (strcmp(str, "LOW") == 0) return LOW;
    if (strcmp(str, "MEDIUM") == 0) return MEDIUM;
    return HIGH;
}
