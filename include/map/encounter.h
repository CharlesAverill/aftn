/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Description
*/

#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include <stdlib.h>

typedef enum {
    QUIET,
    ALIEN_Lost_The_Signal,
    ALIEN_Stalk,
    ALIEN_Hunt,
    ORDER937_Meet_Me_In_The_Infirmary,
    ORDER937_Crew_Expendable,
    ORDER937_Collating_Data
} ENCOUNTER_TYPES;

extern char *encounter_names[7];

#define ENCOUNTER_STACK_SIZE 21
extern int num_encounters;
extern ENCOUNTER_TYPES encounters[ENCOUNTER_STACK_SIZE];
extern ENCOUNTER_TYPES discard_encounters[ENCOUNTER_STACK_SIZE];

void shuffle_encounters();

int draw_encounter();
int discard_encounter();
void replace_encounter();

void replace_alien_cards();
void replace_order937_cards();

#endif
