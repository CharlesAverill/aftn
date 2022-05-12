/**
 * @file
 * @author Charles Averill
 * @date   12-May-2022
 * @brief Description
*/

#ifndef MANAGER_H
#define MANAGER_H

#include "arguments.h"
#include "character.h"
#include "map/map.h"
#include "map/room.h"

typedef struct game_manager game_manager;
struct game_manager {
    int morale;

    map *game_map;

    room *xenomorph_location;
    room *ash_location;

    int round_index;
    int turn_index;
    struct character *characters[5];
    struct character *active_character;
};

game_manager *new_game(const arguments args, map *game_map);

void game_loop(game_manager *manager);

#endif
