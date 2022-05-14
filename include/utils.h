/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief String and integer util function headers, and the room_queue structure
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <strings.h>

#include "map/room.h"

int randint(int low, int high);

void strip_string(char *str, int len);
void trim_string(char *str, int size);

bool is_letter(char c);
bool is_number(char c);

int max(int a, int b);
int min(int a, int b);

char get_character();

// Used for Xeno and Ash trajectory planning
struct room_queue {
    int size;
    int max_size;
    room *head;
    room *tail;
};

struct room_queue *new_room_queue(int capacity);
void print_queue(struct room_queue *q);
int push(struct room_queue *q, room *node);
room *pop(struct room_queue *q);
room *pop_tail(struct room_queue *q);
bool queue_contains(struct room_queue *q, room *target);

#endif
