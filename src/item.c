/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Description
*/

#include "item.h"

int item_costs[NUM_ITEM_TYPES] = {2, 3, 3, 4, 3, 1, -1};

char *item_names[NUM_ITEM_TYPES] = {
    "FLASHLIGHT",
    "MOTION TRACKER",
    "GRAPPLE GUN",
    "INCINERATOR",
    "ELECTRIC PROD",
    "CAT CARRIER",
    "COOLANT CANISTER",
};

int item_uses[NUM_ITEM_TYPES] = {-1, -1, 2, 2, 2, 1, 1};

/**
 * Print the details of an existing item
 * @param i  Pointer to item to print
 */
void print_item(item *i)
{
    if (i == NULL) {
        printf("NONE\n");
    } else {
        printf("%s: %d uses\n", item_names[i->type], i->uses);
    }
}

/**
 * Print the details of an item type
 * @param type  Type of item to print
 */
void print_item_type(ITEM_TYPES type, int discount)
{
    printf("%s: Costs %d Scrap, %d uses\n",
           item_names[type],
           item_costs[type] - discount,
           item_uses[type]);
}

/**
 * Instantiate a new item of type `type`
 * @param  type               Type of item to instantiate
 * @return      Pointer to instantiated item
 */
item *new_item(ITEM_TYPES type)
{
    item *out = (item *)malloc(sizeof(item));
    out->type = type;
    out->uses = item_uses[type];
    return out;
}
