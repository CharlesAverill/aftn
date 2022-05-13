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
     {0, 0, 0},
     0,
     0,
     "Spend an action: Move another crewmember 1 SPACE",
     ripley_ability},
    {"Dallas", "Arthur", "Captain", 5, 0, 0, {0, 0, 0}, 0, 0, "None", dallas_ability},
    {"Parker",
     "Dennis",
     "Chief Engineer",
     4,
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
     {0, 0, 0},
     0,
     0,
     "Spend an action: Look at the upcoming encounter, you may move it to later.",
     lambert_ability},
};

/**
 * The following are character abilities. Each take in the game map, the characters list, and the
 * active character.
 */

void ripley_ability(map *game_map, character characters[5], character *active_character)
{
}

void dallas_ability(map *game_map, character characters[5], character *active_character)
{
}

void parker_ability(map *game_map, character characters[5], character *active_character)
{
    active_character->n_scrap++;
    active_character->current_actions--;
}

void brett_ability(map *game_map, character characters[5], character *active_character)
{
}

void lambert_ability(map *game_map, character characters[5], character *active_character)
{
}
