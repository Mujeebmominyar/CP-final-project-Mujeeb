#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "simulation.h"
#include "log.h"
#include "map.h"
#include "unit.h"
#include "incident.h"

int main() {
    printf("Emergency Dispatch Simulation System\n");
    printf("====================================\n\n");
    
    // Initialize logging system
    init_log();
    
    // Load configuration
    printf("Loading configuration...\n");
    load_configuration("sample_config.txt");
    
    // Log simulation start
    log_simulation_start();
    
    printf("Configuration loaded successfully!\n");
    printf("Map size: %dx%d\n", map_width, map_height);
    printf("Departments loaded: %d\n\n", department_count);
    
    // Start simulation
    printf("Starting simulation...\n\n");
    simulation_loop();
    
    // Log simulation end
    log_simulation_end();
    
    printf("\nSimulation ended. Check log file for detailed events.\n");
    
    // Close log file
    if (log_file) {
        fclose(log_file);
    }
    
    return 0;
}
