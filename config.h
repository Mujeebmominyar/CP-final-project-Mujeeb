#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

void load_configuration(const char *filename);
IncidentType parse_department_type(const char *str);
Priority parse_priority(const char *str);

#endif // CONFIG_H
