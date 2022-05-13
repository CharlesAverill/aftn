/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Description
*/

#include "encounter.h"

#include "manager.h"

int num_encounters = ENCOUNTER_STACK_SIZE;
ENCOUNTER_TYPES encounters[ENCOUNTER_STACK_SIZE] = {
    QUIET,
    QUIET,
    QUIET,
    QUIET,
    QUIET,
    QUIET,
    QUIET,
    QUIET,
    QUIET,
    QUIET,
    QUIET,
    ALIEN_Lost_The_Signal,
    ALIEN_Stalk,
    ALIEN_Stalk,
    ALIEN_Stalk,
    ALIEN_Hunt,
    ALIEN_Hunt,
    ORDER937_Meet_Me_In_The_Infirmary,
    ORDER937_Crew_Expendable,
    ORDER937_Collating_Data,
    ORDER937_Collating_Data,
};

ENCOUNTER_TYPES discard_encounters[ENCOUNTER_STACK_SIZE] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

void shuffle_encounters()
{
    for (int i = ENCOUNTER_STACK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int tmp = encounters[i];
        encounters[i] = encounters[j];
        encounters[j] = tmp;
    }
}

// Returns index of drawn encounter in discard_encounters
int draw_encounter()
{
    if (num_encounters <= 0) {
        return -1;
    }

    return discard_encounter();
}

// Moves card from top of encounters stack to discard_encounters stack
int discard_encounter()
{
    discard_encounters[ENCOUNTER_STACK_SIZE - num_encounters] = encounters[num_encounters - 1];
    encounters[num_encounters - 1] = -1;
    num_encounters--;
    return ENCOUNTER_STACK_SIZE - num_encounters - 1;
}

void replace_encounter()
{
    encounters[num_encounters] = discard_encounters[ENCOUNTER_STACK_SIZE - num_encounters - 1];
    discard_encounters[ENCOUNTER_STACK_SIZE - num_encounters - 1] = -1;
    num_encounters++;
}

void replace_alien_cards()
{
}

void replace_order937_cards()
{
}
