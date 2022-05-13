/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include "map/room.h"

void strip_string(char *str, int len);
void trim_string(char *str, int size);

int is_letter(char c);
int is_number(char c);

int max(int a, int b);
int min(int a, int b);

// Used for Xebo and Ash BFS
struct room_queue {
    int size;
    int max_size;
    room *head;
    room *tail;
};

struct room_queue *newQueue(int capacity);
void print_queue(struct room_queue *q);
int push(struct room_queue *q, room *node);
room *pop(struct room_queue *q);
room *pop_tail(struct room_queue *q);
int queue_contains(struct room_queue *q, room *target);

#endif
