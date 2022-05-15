/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Logic and data for items
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

char *item_desc[NUM_ITEM_TYPES] = {
    "If Morale is lost, reduce the loss byu 2. May be used when it's not your turn. Doesn't stack.",
    "Reveal an Event up to two spaces away.",
    "If the Xenomorph is within three spaces of you, move it up to three spaces in any direction.",
    "If the Xenomorph is within three spaces of you, send it back to the nest.",
    "If the Xenomorph is encountered, reduce Morale lost by 2. May be used when it's not your turn.",
    "If you are surprised by Jonesy, capture him in this for the rest of the game.",
    "May be converted to 2 scrap at WORKSHOP"};

bool item_uses_actions[NUM_ITEM_TYPES] = {false, true, true, true, false, false};

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
        printf("%s:", item_names[i->type]);
        if (i->uses >= 0) {
            printf(" %d uses - ", i->uses);
        } else {
            printf(" inf uses - ");
        }
        printf("%s\n", item_desc[i->type]);
    }
}

/**
 * Print the details of an item type
 * @param type  Type of item to print
 */
void print_item_type(ITEM_TYPES type, int discount)
{
    printf("%s: Costs %d Scrap", item_names[type], item_costs[type] - discount);
    if (item_uses[type] >= 0) {
        printf(", %d uses\n", item_uses[type]);
    } else {
        printf(", inf uses\n");
    }
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
    out->uses_action = item_uses_actions[type];
    out->uses = item_uses[type];
    return out;
}
