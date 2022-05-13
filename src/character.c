/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Description
*/

#include "character.h"

character characters[5] = {
    {"Ripley",
     "Ellen",
     "Warrant Officer",
     4,
     0,
     0,
     0,
     {0, 0, 0},
     0,
     0,
     "Spend an action: Move another crewmember 1 SPACE",
     ripley_ability},
    {"Dallas", "Arthur", "Captain", 5, 0, 0, 0, {0, 0, 0}, 0, 0, "None", dallas_ability},
    {"Parker",
     "Dennis",
     "Chief Engineer",
     4,
     0,
     0,
     0,
     {0, 0, 0},
     0,
     0,
     "Spend an action: Add 1 Scrap to your inventory from the Scrap pile. Use only once per turn.",
     parker_ability},
    {"Brett",
     "Samson",
     "Engineering Tech",
     3,
     0,
     0,
     0,
     {0, 0, 0},
     0,
     0,
     "Reduce cost of any item that costs 2 or more Scrap by 1 Scrap. Items don't take an action to "
     "craft.",
     brett_ability},
    {"Lambert",
     "Joan",
     "Navigator",
     4,
     0,
     0,
     0,
     {0, 0, 0},
     0,
     0,
     "Spend an action: Look at the upcoming encounter, you may discard it.",
     lambert_ability},
};

ability_output *new_ability_output()
{
    ability_output *out = (ability_output *)malloc(sizeof(ability_output));
    out->use_action = true;
    out->can_use_ability_again = true;
    out->move_character_index = -1;
    return out;
}

/**
 * The following are character abilities. Each take in the game map, the characters list, and the
 * active character.
 */

ability_output *ripley_ability(map *game_map, character *characters[5], character *active_character)
{
    ability_output *out = new_ability_output();

    printf("Pick a character to move:\n");
    int i;
    for (i = 0; i < 5; i++) {
        if (characters[i] != NULL) {
            printf("\t%d) %s at %s\n",
                   i + 1,
                   characters[i]->last_name,
                   characters[i]->current_room->name);
        } else {
            break;
        }
    }
    printf("\tb) Back\n");

    char ch = '\0';
    while (ch < '0' || ch > '0' + i) {
        ch = get_character();

        if (ch == 'b') {
            break;
        }
    }

    if (ch == 'b') {
        out->use_action = false;
        return out;
    } else {
        ch = ch - '0' - 1;
        out->move_character_index = ch;
    }

    return out;
}

ability_output *dallas_ability(map *game_map, character *characters[5], character *active_character)
{
    ability_output *out = new_ability_output();
    out->use_action = false;

    //printf("%s has no ability\n", active_character->last_name);

    return out;
}

ability_output *parker_ability(map *game_map, character *characters[5], character *active_character)
{
    ability_output *out = new_ability_output();
    out->can_use_ability_again = false;

    active_character->num_scrap++;

    return out;
}

ability_output *brett_ability(map *game_map, character *characters[5], character *active_character)
{
}

ability_output *
lambert_ability(map *game_map, character *characters[5], character *active_character)
{
    ability_output *out = new_ability_output();

    printf("Confirm use of this ability? (y/n) ");

    char ch;
    while (ch != 'y' && ch != 'n') {
        ch = get_character();
    }

    if (ch == 'n') {
        out->use_action = false;
        return out;
    }

    int discard_index = draw_encounter();
    ENCOUNTER_TYPES encounter = discard_encounters[discard_index];

    printf("Drawn encounter: %s\n", encounter_names[encounter]);

    printf("Discard this encounter? (y/n) ");

    ch = '\0';
    while (ch != 'y' && ch != 'n') {
        ch = get_character();
    }

    if (ch == 'n') {
        replace_encounter();
    }

    return out;
}
