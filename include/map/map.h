/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief The map structure and accompanying function headers
*/

#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map/room.h"
#include "utils.h"

// A structure to store rooms and corridors, along with auxiliary data representations
typedef struct map {
    // This map's name
    char name[32];

    // The number of rooms in this map
    int room_count;
    // An array of pointers to the rooms in this map
    room *rooms[64];

    // The number of named rooms (not corridors) in this map
    int named_room_count;
    // The indices of named rooms in `rooms`
    int named_room_indices[64];

    // The room playable characters are intended to start in
    room *player_start_room;
    // The room the xenomorph starts in and can respawn to
    room *xenomorph_start_room;
    // The room Ash starts in and playable characters can respawn to
    room *ash_start_room;

    // The number of rooms that start with scrap
    int scrap_room_count;
    // Pointers to the rooms that start with scrap
    room *scrap_rooms[8];
    // The number of rooms that start with events
    int event_room_count;
    // Pointers to the rooms that start with events
    room *event_rooms[8];
    // The number of rooms that start with coolant
    int coolant_room_count;
    // Pointers to the rooms that start with events
    room *coolant_rooms[8];

    // An ascii representation of the map (optional)
    char *ascii_map;
} map;

// A list of rooms returned by DFSs
// TODO : This is a clone of room_queue, move room_queue here and replace this
typedef struct dfs_results {
    // The rooms found by the DFS
    room *rooms[64];
    // The number of rooms found by the DFS
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
