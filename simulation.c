#include "simulation.h"
#include "incident.h"
#include "unit.h"
#include "map.h"
#include "log.h"
#include "config.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static int current_turn = 0;

void show_menu() {
    printf("\n=== SIMULATION MENU ===\n");
    printf("1. Enter a new incident\n");
    printf("2. Continue simulation\n");
    printf("3. Show status report\n");
    printf("4. Exit program\n");
    printf("Choose an option (1-4): ");
}

void add_new_incident() {
    int x, y, type_int, priority_int;
    IncidentType type;
    Priority priority;

    printf("\nEnter incident details:\n");
    printf("X coordinate (0-%d): ", map_width - 1);
    if (scanf("%d", &x) != 1) {
        printf("Invalid input for X coordinate!\n");
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    printf("Y coordinate (0-%d): ", map_height - 1);
    if (scanf("%d", &y) != 1) {
        printf("Invalid input for Y coordinate!\n");
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    printf("Incident type (0=FIRE, 1=MEDICAL, 2=POLICE): ");
    if (scanf("%d", &type_int) != 1) {
        printf("Invalid input for incident type!\n");
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    printf("Priority (0=LOW, 1=MEDIUM, 2=HIGH): ");
    if (scanf("%d", &priority_int) != 1) {
        printf("Invalid input for priority!\n");
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    // Validate input
    if (x < 0 || x >= map_width || y < 0 || y >= map_height) {
        printf("Invalid coordinates! X must be 0-%d, Y must be 0-%d\n", map_width - 1, map_height - 1);
        return;
    }

    if (type_int < 0 || type_int > 2 || priority_int < 0 || priority_int > 2) {
        printf("Invalid type or priority! Type must be 0-2, Priority must be 0-2\n");
        return;
    }

    type = (IncidentType)type_int;
    priority = (Priority)priority_int;

    // Add incident
    ensure_incident_capacity();

    Incident *new_incident = &incidents[incident_count];
    new_incident->x = x;
    new_incident->y = y;
    new_incident->type = type;
    new_incident->priority = priority;
    new_incident->state = INCIDENT_WAITING;

    // Initialize operation turns based on incident requirements
    IncidentRequirement req = incident_requirements[type][priority];
    new_incident->operation_turns_remaining = req.time_required;

    // Initialize dispatched units arrays
    for (int i = 0; i < 3; i++) {
        new_incident->dispatched_count[i] = 0;
        for (int j = 0; j < MAX_REQUIRED_UNITS_PER_TYPE; j++) {
            new_incident->dispatched_units[i][j] = NULL;
        }
    }

    incident_count++;

    printf("Incident created successfully at (%d,%d): %s priority %s\n",
           x, y, getIncidentTypeStr(type), getPriorityStr(priority));

    log_incident_created(type, priority, x, y);
}

void show_status_report() {
    printf("\n=== STATUS REPORT ===\n");
    printf("Turn: %d\n", current_turn);
    printf("Active incidents: %d\n", incident_count);

    // Show incidents
    printf("\nIncidents:\n");
    for (int i = 0; i < incident_count; i++) {
        if (incidents[i].state != INCIDENT_FINISHED) {
            printf("  [%d] %s %s at (%d,%d) - %s\n",
                   i, getPriorityStr(incidents[i].priority),
                   getIncidentTypeStr(incidents[i].type),
                   incidents[i].x, incidents[i].y,
                   getIncidentStateStr(incidents[i].state));
        }
    }

    // Show departments and units
    printf("\nDepartments and Units:\n");
    for (int i = 0; i < department_count; i++) {
        printf("  Department %s-%d at (%d,%d):\n",
               getUnitTypeStr(departments[i].type),
               departments[i].number,
               departments[i].x, departments[i].y);

        for (int j = 0; j < departments[i].unit_count; j++) {
            Unit *u = &departments[i].units[j];
            printf("    Unit %d-%d at (%d,%d) - %s\n",
                   u->departmentNumber, u->unitNumber,
                   u->x, u->y, getUnitStateStr(u->state));
        }
    }
}

int process_user_input() {
    int choice;

    while (1) {
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number (1-4): ");
            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        switch (choice) {
            case 1:
                add_new_incident();
                printf("Returning to menu...\n");
                return 0; // Return to menu

            case 2:
                printf("Continuing simulation...\n");
                return 1; // Continue to simulation

            case 3:
                show_status_report();
                printf("\nPress Enter to return to menu...");
                // Wait for Enter key
                char input;
                scanf("%c", &input);
                return 0; // Return to menu

            case 4:
                printf("Exiting simulation...\n");
                exit(0);

            default:
                printf("Invalid choice! Please enter a number between 1-4: ");
                break;
        }
    }
}

void perform_turn_actions() {
    printf("\n--- Turn %d ---\n", current_turn);

    // 1. Dispatch units to incidents
    dispatch_units();

    // 2. Update unit movements
    update_units_movement();

    // 3. Update incident states
    update_incidents();

    // 4. Render the map
    update_and_render();

    // Small delay for visualization
    printf("\nPress Enter to continue to next turn...");
    // Wait for Enter key
    char input;
    scanf("%c", &input);
}

void simulation_loop() {
    current_turn = 0;

    while (1) {
        // Show menu every 10 turns or at the beginning
        if (current_turn % 10 == 0) {
            int continue_simulation = 0;

            // Keep showing menu until user chooses to continue simulation
            while (!continue_simulation) {
                show_menu();
                continue_simulation = process_user_input();
            }
        }

        // Perform turn actions
        perform_turn_actions();

        current_turn++;
    }
}
