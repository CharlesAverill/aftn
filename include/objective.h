/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   13-May-2022
 * @brief Description
*/

#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include <stdbool.h>
#include <stdlib.h>

#include "item.h"
#include "map/room.h"

typedef enum { BRING_ITEM_TO_LOCATION, CREW_AT_LOCATION_WITH_MINIMUM_SCRAP, DROP_COOLANT } OBJECTIVE_TYPES;

typedef struct objective {
    char name[64];
    OBJECTIVE_TYPES type;
    bool completed;

    // For BRING_ITEM_TO_LOCATION and DROP_COOLANT
    char location_name[32];
    room *location;

    // For BRING_ITEM_TO_LOCATION
    ITEM_TYPES target_item_type;

    // For CREW_AT_LOCATION_WITH_MINIMUM_SCRAP
    int minimum_scrap;
} objective;

#define NUM_OBJECTIVES 10
extern objective objectives_stack[NUM_OBJECTIVES];

objective *get_objectives(int n);

void print_objective_description(objective o);

#endif
