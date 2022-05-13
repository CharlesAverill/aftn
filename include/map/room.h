/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#ifndef ROOM_H
#define ROOM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct room room;
struct room { // This forward declaration allows for a room pointer in the struct definition
    char name[32];
    bool is_corridor;

    int num_scrap;
    bool has_event;

    int connection_count;
    room *connections[8];
    room *ladder_connection;

    int search_distance;
    bool search_discovered;
    room *search_previous_room;

    room *room_queue_next;
};

room *create_room(char name[32], bool is_corridor);
void add_connection(room *a, room *b);

void print_room(room *r, bool prepend_tab);

#endif
