/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#include "utils.h"

#include <string.h>
#include <strings.h>

void strip_string(char *str, int len)
{
    int index = len - 1;

    while (str[index] == ' ' || str[index] == '\n' || str[index] == '\t') {
        index--;
    }

    str[index + 1] = '\0';
}

void trim_string(char *str, int size)
{
    if (size < 0) { // trim abs(size) from front of string
        memmove(str, str + (size * -1), strlen(str));
    } else { // trim abs(size) from end of string
        int len = strlen(str);
        str[len - size] = '\0';
    }
}

int is_letter(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_number(char c)
{
    return (c >= '0' && c <= '9');
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

struct room_queue *newQueue(int capacity)
{
    struct room_queue *q;
    q = malloc(sizeof(struct room_queue));

    if (q == NULL) {
        return q;
    }

    q->size = 0;
    q->max_size = capacity;
    q->head = NULL;
    q->tail = NULL;

    return q;
}

void print_queue(struct room_queue *q)
{
    room *tmp = q->head;

    while (tmp != NULL) {
        printf("%s ", tmp->name);
        tmp = tmp->room_queue_next;
    }

    printf("\n");
}

int push(struct room_queue *q, room *node)
{
    if ((q->size + 1) > q->max_size) {
        return q->size;
    }

    if (node == NULL) {
        return q->size;
    }

    node->room_queue_next = NULL;

    if (q->head == NULL) {
        q->head = node;
        q->tail = node;
        q->size = 1;

        return q->size;
    }

    q->tail->room_queue_next = node;
    q->tail = node;
    q->size += 1;

    return q->size;
}

room *pop(struct room_queue *q)
{
    if (q->size == 0) {
        return NULL;
    }

    room *tmp = q->head;
    q->head = q->head->room_queue_next;
    q->size -= 1;

    return tmp;
}

room *pop_tail(struct room_queue *q)
{
    if (q->size == 0) {
        return NULL;
    }

    room *tmp = q->head;
    while (tmp->room_queue_next != q->tail) {
        tmp = tmp->room_queue_next;
    }
    room *out = tmp->room_queue_next;
    tmp->room_queue_next = NULL;

    q->tail = tmp;

    return tmp;
}

int queue_contains(struct room_queue *q, room *target)
{
    room *tmp = q->head;
    while (tmp != NULL) {
        if (tmp == target) {
            return 1;
        }
        tmp = tmp->room_queue_next;
    }
    return 0;
}
