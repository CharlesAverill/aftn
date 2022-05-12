/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#ifndef MAP_H
#define MAP_H

#include "room.h"

typedef struct map {
    char name[32];

    int room_count;
    room *rooms[64];

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

dfs_results *new_dfs_results();
void reset_dfs_distances(map *game_map);
dfs_results *find_rooms_by_distance(map *game_map, room *start_room, int distance, int inclusive);

map *read_map(const char *fn);
void print_map(const map *game_map);

#endif
