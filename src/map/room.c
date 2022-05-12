/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map/room.h"

room *create_room(char name[32], int is_corridor)
{
    room *new_room = (room *)malloc(sizeof(room));

    strcpy(new_room->name, name);
    new_room->is_corridor = is_corridor;

    new_room->num_scrap = 0;
    new_room->has_event = 0;

    new_room->connection_count = 0;
    for (int i = 0; i < 8; i++) {
        new_room->connections[i] = NULL;
    }
    new_room->ladder_connection = NULL;

    new_room->dfs_distance = -1;

    return new_room;
}

void add_connection(room *a, room *b)
{
    if (a->connection_count >= 8) {
        fprintf(stderr, "[ERROR] - Room %s has more than 8 connections\n", a->name);
        exit(1);
    } else if (b->connection_count >= 8) {
        fprintf(stderr, "[ERROR] - Room %s has more than 8 connections\n", b->name);
        exit(1);
    }

    int found = 0;
    for (int i = 0; i < a->connection_count; i++) {
        if (a->connections[i] == b) {
            found = 1;
            break;
        }
    }
    if (!found) {
        a->connections[a->connection_count] = b;
        a->connection_count++;
    }

    found = 0;
    for (int i = 0; i < b->connection_count; i++) {
        if (b->connections[i] == a) {
            found = 1;
            break;
        }
    }
    if (!found) {
        b->connections[b->connection_count] = a;
        b->connection_count++;
    }
}

void print_room(room *r, int prepend_tab)
{
    char *prepend = prepend_tab ? "\t" : "";
    printf("%sName: %s\n", prepend, r->name);
    printf("%sType: %s\n", prepend, r->is_corridor ? "Corridor" : "Room");
    printf("%sScrap: %d\n", prepend, r->num_scrap);
    printf("%sHas Event: %s\n", prepend, r->has_event ? "True" : "False");

    printf("%sConnections: ", prepend);
    for (int i = 0; i < r->connection_count; i++) {
        printf("%s ", r->connections[i]->name);
    }
    printf("\n");

    if (r->ladder_connection != NULL) {
        printf("%sLadder Connection: %s\n", prepend, r->ladder_connection->name);
    }
}
