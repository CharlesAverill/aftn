/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief The item structure and accompanying file headers
 */

#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ITEM_TYPES 7

// Enum defining types of items
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
extern char *item_desc[NUM_ITEM_TYPES];
extern int item_uses[NUM_ITEM_TYPES];

// Structure for holding item data
typedef struct item {
  // Type of item
  ITEM_TYPES type;
  // Whether or not this item can be used with an action
  bool uses_action;
  // Number of uses left for this item
  int uses;
} item;

void print_item(item *i);
void print_item_type(ITEM_TYPES type, int discount);

item *new_item(ITEM_TYPES type);

#endif
