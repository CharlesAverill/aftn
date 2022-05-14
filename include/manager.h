/**
 * @file
 * @author Charles Averill
 * @date   12-May-2022
 * @brief The game_manager structure and accompanying function headers
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

// Structure holding all game data
typedef struct game_manager game_manager;
struct game_manager {
    // Team morale, game ends when this reaches 0
    int morale;

    // Number of objectives to complete before the final mission is revealed
    int num_objectives;
    // Array of objectives to complete before the final mission is revealed
    objective *game_objectives;

    // Pointer to the game's map
    map *game_map;

    // Pointer to the xenomorph's current location
    room *xenomorph_location;
    // Pointer to Ash's current location
    room *ash_location;

    // Whether or not Jonesy has been caught in a cat carrier
    bool jonesy_caught;

    // Counter of how many rounds have occurred
    int round_index;
    // Counter of how many of a character's turns have occurred
    int turn_index;

    // Number of characters in the game
    int character_count;
    // Array of characters in the game
    struct character *characters[5];
    // The character that is currently acting
    struct character *active_character;
};

game_manager *new_game(const arguments args, map *game_map);

void print_game_objectives(game_manager *manager);
void update_objectives(game_manager *manager);

room *character_move(game_manager *manager, struct character *to_move, room_queue *allowed_moves, bool allow_back);
bool xeno_move(game_manager *manager, int num_spaces, int morale_drop);
void ash_move(game_manager *manager, int num_spaces);

int reduce_morale(game_manager *manager, int lost, bool encountered_alien);
int trigger_event(game_manager *manager, struct character *moved, room *motion_tracker_room);
void trigger_encounter(game_manager *manager);
void flee(game_manager *manager, struct character *moved);

bool pickup(game_manager *manager);
bool drop(game_manager *manager);
int use(game_manager *manager);

room_queue *shortest_path(map *game_map, room *from, room *to);

void game_loop(game_manager *manager);

#endif
