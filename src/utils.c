/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief String and integer utils, and logic for the room_queue structure
*/

#include "utils.h"

/**
 * Return a random integer in [low, high]
 * @param  low                Low bound, inclusive
 * @param  high               High bound, inclusive
 * @return      A random integer in [low, high]
 */
int randint(int low, int high)
{
    return (rand() % (high - low + 1)) + low;
}

/**
 * Strip whitespace characters from string
 * @param str  String to strip
 * @param len  String length
 */
void strip_string(char *str, int len)
{
    int index = len - 1;

    while (str[index] == ' ' || str[index] == '\n' || str[index] == '\t') {
        index--;
    }

    str[index + 1] = '\0';
}

/**
 * Remove `abs(size)` characters from front or back of `str`
 * @param str   String to trim
 * @param size  Number of characters to trim. If negative, will trim from front, back otherwise
 */
void trim_string(char *str, int size)
{
    if (size < 0) { // trim abs(size) from front of string
        memmove(str, str + (size * -1), strlen(str));
    } else { // trim size from end of string
        int len = strlen(str);
        str[len - size] = '\0';
    }
}

/**
 * Determine if a character is a letter
 * @param  c               Character to analyze
 * @return   True if `c` is a letter, false otherwise
 */
bool is_letter(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

/**
 * Determine if a character is a number
 * @param  c               Character to analyze
 * @return   True if `c` is a number, false otherwise
 */
bool is_number(char c)
{
    return (c >= '0' && c <= '9');
}

/**
 * Determine the maximum of two integers
 * @param  a               First integer to analyze
 * @param  b               Second integer to analyze
 * @return   The larger integer of a and b
 */
int max(int a, int b)
{
    return a > b ? a : b;
}

/**
 * Determine the minimum of two integers
 * @param  a               First integer to analyze
 * @param  b               Second integer to analyze
 * @return   The smaller integer of a and b
 */
int min(int a, int b)
{
    return a < b ? a : b;
}

/**
 * Read character from stdin
 * @return Input character
 */
char get_character()
{
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);

    char ch = line[0];
    free(line);

    return ch;
}

/**
 * Initialize a new room_queue
 * @param  capacity               Maximum capacity of queue
 * @return          A pointer to an initialized room_queue
 */
struct room_queue *new_room_queue(int capacity)
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

/**
 * Prints a queue's elements
 * @param q  Queue to print
 */
void print_queue(struct room_queue *q)
{
    room *tmp = q->head;

    while (tmp != NULL) {
        printf("%s ", tmp->name);
        tmp = tmp->room_queue_next;
    }

    printf("\n");
}

/**
 * Push room onto room_queue
 * @param  q                  Queue to push onto
 * @param  node               Room to push onto queue
 * @return      New size of queue
 */
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

/**
 * Pop from front of room_queue
 * @param  q               Queue to pop from
 * @return   Popped room
 */
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

/**
 * Pop from back of room_queue
 * @param  q               Queue to pop from
 * @return   Popped room
 */
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

/**
 * Check if a room_queue contains `target`
 * @param  q                    Queue to check
 * @param  target               Room to look for
 * @return        True if `target` is in `q`, false otherwise
 */
bool queue_contains(struct room_queue *q, room *target)
{
    room *tmp = q->head;
    while (tmp != NULL) {
        if (tmp == target) {
            return true;
        }
        tmp = tmp->room_queue_next;
    }
    return false;
}
