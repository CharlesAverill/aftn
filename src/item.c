/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Description
*/

#include "item.h"

int item_costs[7] = {2, 3, 3, 4, 3, 1, -1};

char *item_names[7] = {
    "FLASHLIGHT",
    "MOTION TRACKER",
    "GRAPPLE GUN",
    "INCINERATOR",
    "ELECTRIC PROD",
    "CAT CARRIER",
    "COOLANT CANISTER",
};

int item_uses[7] = {-1, -1, 2, 2, 2, 1, 1};

void print_item(item *i)
{
    printf("%s: %d uses\n", item_names[i->type], i->uses);
}

void print_item_type(ITEM_TYPES type)
{
    printf("%s: Costs %d Scrap, %d uses\n", item_names[type], item_costs[type], item_uses[type]);
}
