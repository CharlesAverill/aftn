/**
 * @file
 * @author Charles Averill
 * @date   12-May-2022
 * @brief Description
*/

#ifndef MANAGER_H
#define MANAGER_H

#include <limits.h>
#include <stdlib.h>
#include <time.h>

#include "arguments.h"
#include "character.h"
#include "map/encounter.h"
#include "map/map.h"
#include "map/room.h"
#include "objective.h"
#include "utils.h"

typedef struct game_manager game_manager;
struct game_manager {
    int morale;
    int num_objectives;
    objective *game_objectives;

    map *game_map;

    room *xenomorph_location;
    room *ash_location;

    int round_index;
    int turn_index;

    int character_count;
    struct character *characters[5];
    struct character *active_character;
};

game_manager *new_game(const arguments args, map *game_map);

void print_game_objectives(game_manager *manager);
void update_objectives(game_manager *manager);

room *character_move(game_manager *manager, struct character *to_move, dfs_results *allowed_moves, bool allow_back);
bool xeno_move(game_manager *manager, int num_spaces, int morale_drop);
void ash_move(game_manager *manager, int num_spaces);

int reduce_morale(game_manager *manager, int lost, bool encountered_alien);
int trigger_event(game_manager *manager, struct character *moved);
void trigger_encounter(game_manager *manager);
void flee(game_manager *manager, struct character *moved);

bool pickup(game_manager *manager);
bool drop(game_manager *manager);

struct room_queue *shortest_path(map *game_map, room *from, room *to);

void game_loop(game_manager *manager);

#endif
