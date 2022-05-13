/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Description
*/

#ifndef CHARACTER_H
#define CHARACTER_H

#include "item.h"
#include "manager.h"
#include "map/map.h"
#include "map/room.h"

typedef struct character character;
struct character {
    char last_name[16];
    char first_name[16];
    char rank[32];

    int max_actions;
    int current_actions;

    int n_scrap;
    item *held_items[3];
    item *coolant;

    room *current_room;

    char ability_description[128];
    void (*ability_function)(map *, character *, character *);
};

void ripley_ability(map *game_map, character characters[5], character *active_character);
void dallas_ability(map *game_map, character characters[5], character *active_character);
void parker_ability(map *game_map, character characters[5], character *active_character);
void brett_ability(map *game_map, character characters[5], character *active_character);
void lambert_ability(map *game_map, character characters[5], character *active_character);

extern character characters[5];

#endif
