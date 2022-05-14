/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief The room structure and accompanying file headers
*/

#ifndef ROOM_H
#define ROOM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"

// Structure containing general room data
typedef struct room room;
struct room { // This forward declaration allows for a room pointer in the struct definition
    // Room name
    char name[32];

    // Whether the room is a corridor or not
    bool is_corridor;

    // The number of scrap in this room
    int num_scrap;

    // Whether or not this room has an event
    bool has_event;

    // Number of items in the room
    int num_items;
    // Array of pointers to items in the room
    item *room_items[4];

    // Number of connections to other rooms
    int connection_count;
    // Array of connections to other rooms
    room *connections[8];
    // A connection by ladder
    room *ladder_connection;

    // Data used when searching over a map
    int search_distance;
    bool search_discovered;
    room *search_previous_room;

    // Room pointer used to build linked list of rooms for room queue
    room *room_queue_next;
};

room *create_room(char name[32], bool is_corridor);
void add_connection(room *a, room *b);

void print_room(room *r, bool prepend_tab);

#endif
