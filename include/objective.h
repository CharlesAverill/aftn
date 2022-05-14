/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   13-May-2022
 * @brief The objective structure and accompanying function headers
*/

#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include <stdbool.h>
#include <stdlib.h>

#include "item.h"
#include "map/room.h"

// An enum defining types of objectives
typedef enum { BRING_ITEM_TO_LOCATION, CREW_AT_LOCATION_WITH_MINIMUM_SCRAP, DROP_COOLANT } OBJECTIVE_TYPES;

// A structure to store objective data
typedef struct objective {
    // This objective's name
    char name[64];
    // This objective's objective type
    OBJECTIVE_TYPES type;
    // Whether or not this objective has been completed
    bool completed;

    // For BRING_ITEM_TO_LOCATION and DROP_COOLANT
    // Location name to bring items to or drop coolant at
    char location_name[32];
    // Pointer to room to bring items to or drop coolant at
    room *location;

    // For BRING_ITEM_TO_LOCATION
    // Target item type to bring to location
    ITEM_TYPES target_item_type;

    // For CREW_AT_LOCATION_WITH_MINIMUM_SCRAP
    // Number of scrap for each character to have at the given location
    int minimum_scrap;
} objective;

#define NUM_OBJECTIVES 10
extern objective objectives_stack[NUM_OBJECTIVES];

objective *get_objectives(int n);

void complete_objective(objective *o);

void print_objective_description(objective o);

#endif
