/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Description
*/

#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ITEM_TYPES 7

typedef enum {
    FLASHLIGHT,
    MOTION_TRACKER,
    GRAPPLE_GUN,
    INCINERATOR,
    ELECTRIC_PROD,
    CAT_CARRIER,
    COOLANT_CANISTER
} ITEM_TYPES;

extern int item_costs[NUM_ITEM_TYPES];
extern char *item_names[NUM_ITEM_TYPES];
extern int item_uses[NUM_ITEM_TYPES];

typedef struct item {
    ITEM_TYPES type;
    bool uses_action;
    int uses;
} item;

void print_item(item *i);
void print_item_type(ITEM_TYPES type, int discount);

item *new_item(ITEM_TYPES type);

#endif
