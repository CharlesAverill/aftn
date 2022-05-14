/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   13-May-2022
 * @brief Description
*/

#include "objective.h"

objective objectives_stack[NUM_OBJECTIVES] = {
    {"PREP SUITS", DROP_COOLANT, false, "SUIT STORAGE", 0, 0, 0},
    {"WE'LL TAKE OUR CHANCES IN THE SHUTTLE", DROP_COOLANT, false, "DOCKING BAY", 0, 0, 0},
    {"CREW MEETING", CREW_AT_LOCATION_WITH_MINIMUM_SCRAP, false, "GALLEY", 0, 0, 1},
    {"WHAT'S THE DAMAGE?", CREW_AT_LOCATION_WITH_MINIMUM_SCRAP, false, "WORKSHOP", 0, 0, 0},
    {"DRIVE 'EM INTO THE AIRLOCK", BRING_ITEM_TO_LOCATION, false, "AIRLOCK", 0, INCINERATOR, 0},
    {"WHERE IS IT?", BRING_ITEM_TO_LOCATION, false, "MED BAY", 0, FLASHLIGHT, 0},
    {"SHOULDN'T HAVE LANDED ON THIS BALL", BRING_ITEM_TO_LOCATION, false, "GARAGE", 0, GRAPPLE_GUN, 0},
    {"ENCOUNTER THE NEST", BRING_ITEM_TO_LOCATION, false, "NEST", 0, INCINERATOR, 0},
    {"CHECK THE HYPERSLEEP CHAMBER", BRING_ITEM_TO_LOCATION, false, "HYPERSLEEP", 0, MOTION_TRACKER, 0},
    {"GIVE IT A LITTLE INCENTIVE", BRING_ITEM_TO_LOCATION, false, "GALLEY", 0, ELECTRIC_PROD, 0},
};

/**
 * Get an array of n random objectives
 * @param  n               Number of objectives to get
 * @return   An array of objectives allocated on the heap
 */
objective *get_objectives(int n)
{
    if (n < 1 || n > NUM_OBJECTIVES) {
        printf("[ERROR] - Number of objectives must be in [1, %d]\n", NUM_OBJECTIVES);
        exit(1);
    }

    objective *out = (objective *)malloc(sizeof(objective) * n);

    // Shuffle objectives
    for (int i = NUM_OBJECTIVES - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        objective tmp = objectives_stack[i];
        objectives_stack[i] = objectives_stack[j];
        objectives_stack[j] = tmp;
    }

    for (int i = 0; i < n; i++) {
        out[i] = objectives_stack[i];
    }

    return out;
}

void print_objective_description(objective o)
{
    printf("%s: ", o.name);

    switch (o.type) {
    case BRING_ITEM_TO_LOCATION:
        printf("Bring %s to %s\n", item_names[o.target_item_type], o.location->name);
        break;
    case CREW_AT_LOCATION_WITH_MINIMUM_SCRAP:
        printf("All Crew members in %s", o.location->name);
        if (o.minimum_scrap > 0) {
            printf(" with at least %d scrap in each Crew member's inventory.\n", o.minimum_scrap);
        } else {
            printf("\n");
        }
        break;
    case DROP_COOLANT:
        printf("Drop 2 COOLANT CANISTERS in %s\n", o.location->name);
        break;
    }
}
