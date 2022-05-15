/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Logic and data for encounters
*/

#include "map/encounter.h"

char *encounter_names[7] = {"QUIET",
                            "ALIEN - Lost The Signal",
                            "ALIEN - Stalk",
                            "ALIEN - Hunt",
                            "ORDER 937 - Meet Me In The Infirmary",
                            "ORDER 937 - Crew Expendable",
                            "ORDER 937 - Collating Data"};

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

ENCOUNTER_TYPES discard_encounters[ENCOUNTER_STACK_SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                                            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

/**
 * Shuffle the encounters deck
 */
void shuffle_encounters()
{
    for (int i = num_encounters; i > 0; i--) {
        int j = rand() % (i + 1);

        ENCOUNTER_TYPES tmp = encounters[i];
        encounters[i] = encounters[j];
        encounters[j] = tmp;
    }
}

/**
 * Draws an encounter card, discards it, and returns the index of the card in discard_encounters
 * @return Index of the card in discard_encounters
 */
int draw_encounter()
{
    if (num_encounters <= 0) {
        return -1;
    }

    return discard_encounter();
}

/**
 * Moves card from top of encounters stack to top of discard_encounters stack
 * @return index of discarded card in discard_encounters
 */
int discard_encounter()
{
    if (num_encounters <= 0) {
        return -1;
    }

    discard_encounters[ENCOUNTER_STACK_SIZE - num_encounters] = encounters[num_encounters - 1];
    encounters[num_encounters - 1] = -1;
    num_encounters--;
    return ENCOUNTER_STACK_SIZE - num_encounters - 1;
}

/**
 * Moves card from top of discard_encounters stack to top of encounters stack
 */
void replace_encounter()
{
    if (num_encounters >= ENCOUNTER_STACK_SIZE) {
        return;
    }

    encounters[num_encounters] = discard_encounters[ENCOUNTER_STACK_SIZE - num_encounters - 1];
    discard_encounters[ENCOUNTER_STACK_SIZE - num_encounters - 1] = -1;
    num_encounters++;
}

/**
 * Put all encounter cards back in the stack
 */
void replace_all_encounters()
{
    while (num_encounters < ENCOUNTER_STACK_SIZE) {
        replace_encounter();
    }
    shuffle_encounters();
}

/**
 * Replace a discarded card at position i
 * @param idx  Index in discarded pile
 * @return     Whether or not the replace was successful
 */
bool replace_card(int idx)
{
    if (num_encounters >= ENCOUNTER_STACK_SIZE) {
        return false;
    }

    encounters[num_encounters] = discard_encounters[idx];
    discard_encounters[idx] = -1;
    num_encounters++;

    for (int i = 0; i < ENCOUNTER_STACK_SIZE - 1; i++) {
        if (discard_encounters[i] == -1) {
            int j;
            for (j = i; j < ENCOUNTER_STACK_SIZE; j++) {
                if (discard_encounters[j] != -1) {
                    break;
                }
            }
            if (j == ENCOUNTER_STACK_SIZE - 1) {
                break;
            }
            discard_encounters[i] = discard_encounters[j];
        }
    }

    return true;
}

/**
 * Replaces all alien cards in discard_encounters
 */
void replace_alien_cards()
{
    for (int i = 0; i < ENCOUNTER_STACK_SIZE - num_encounters; i++) {
        if (discard_encounters[i] >= ALIEN_Lost_The_Signal && discard_encounters[i] <= ALIEN_Hunt) {
            replace_card(i);
            i--;
        }
    }
    shuffle_encounters();
}

/**
 * Replaces all order937 cards in discard_encounters
 */
void replace_order937_cards()
{
    for (int i = 0; i < ENCOUNTER_STACK_SIZE - num_encounters; i++) {
        if (discard_encounters[i] >= ORDER937_Meet_Me_In_The_Infirmary &&
            discard_encounters[i] <= ORDER937_Collating_Data) {
            replace_card(i);
            i--;
        }
    }
    shuffle_encounters();
}
