/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#ifndef ROOM_H
#define ROOM_H

typedef struct room room;
struct room { // This forward declaration allows for a room pointer in the struct definition
    char name[32];
    int is_corridor;

    int num_scrap;
    int has_event;

    int connection_count;
    room *connections[8];
    room *ladder_connection;
};

room *create_room(char name[32], int is_corridor);
void add_connection(room *a, room *b);

void print_room(room *r, int prepend_tab);

#endif
