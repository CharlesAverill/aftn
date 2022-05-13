/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Description
*/

#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    FLASHLIGHT,
    MOTION_TRACKER,
    GRAPPLE_GUN,
    INCINERATOR,
    ELECTRIC_PROD,
    CAT_CARRIER,
    COOLANT_CANISTER
} ITEM_TYPES;

extern int item_costs[7];
extern char *item_names[7];
extern int item_uses[7];

typedef struct item {
    ITEM_TYPES type;
    int uses;
} item;

void print_item(item *i);
void print_item_type(ITEM_TYPES type);

item *new_item(ITEM_TYPES type);

#endif
