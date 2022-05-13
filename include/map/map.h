/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map/room.h"
#include "utils.h"

typedef struct map {
    char name[32];

    int room_count;
    room *rooms[64];

    int named_room_count;
    int named_room_indices[64];

    room *player_start_room;
    room *xenomorph_start_room;
    room *ash_start_room;

    int scrap_room_count;
    room *scrap_rooms[8];
    int event_room_count;
    room *event_rooms[8];
    int coolant_room_count;
    room *coolant_rooms[8];

    char *ascii_map;
} map;

typedef struct dfs_results {
    room *rooms[64];
    int num_results;
} dfs_results;

room *get_room(map *game_map, const char *room_name);
int get_room_index(map *game_map, const char *room_name);
room *add_room_if_not_exists(map *game_map, char *room_name);

dfs_results *new_dfs_results();
void reset_search(map *game_map, int distance);
dfs_results *find_rooms_by_distance(map *game_map, room *start_room, int distance, bool inclusive);

map *read_map(const char *fn);
void print_map(const map *game_map);

#endif
