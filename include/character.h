/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Description
*/

#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdbool.h>

#include "item.h"
#include "manager.h"
#include "map/map.h"
#include "map/room.h"
#include "utils.h"

typedef struct ability_output {
    bool use_action;
    bool can_use_ability_again;
    int move_character_index;
} ability_output;

typedef struct character character;
struct character {
    char last_name[16];
    char first_name[16];
    char rank[32];

    int max_actions;
    int current_actions;

    int num_scrap;
    int num_items;
    item *held_items[3];
    item *coolant;

    room *current_room;

    char ability_description[128];
    ability_output *(*ability_function)(map *, character **, character *);
};

bool character_has_item(character *c, ITEM_TYPES type);

ability_output *ripley_ability(map *game_map, character *characters[5], character *active_character);
ability_output *dallas_ability(map *game_map, character *characters[5], character *active_character);
ability_output *parker_ability(map *game_map, character *characters[5], character *active_character);
ability_output *brett_ability(map *game_map, character *characters[5], character *active_character);
ability_output *lambert_ability(map *game_map, character *characters[5], character *active_character);

extern character characters[5];

ability_output *new_ability_output();

#endif
