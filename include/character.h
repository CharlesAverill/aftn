/**
 * @file
 * @author Charles Averill <charlesaverill>
 * @date   12-May-2022
 * @brief Character structures and corresponding function headers
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdbool.h>

#include "item.h"
#include "manager.h"
#include "map/map.h"
#include "map/room.h"
#include "utils.h"

// A structure to pass data from a character's ability_function to the game loop
typedef struct ability_output {
  // Whether or not this ability used an action
  bool use_action;
  // Whether or not this ability may be used in the same turn
  bool can_use_ability_again;
  // If a character was moved, the index of character that has been moved
  int move_character_index;
} ability_output;

// A structure to hold general character data
typedef struct character character;
struct character {
  // General cosmetic data
  char last_name[16];
  char first_name[16];
  char rank[32];

  // The maximum number of actions per turn this character can take
  int max_actions;
  // A counter for the number of used actions during a turn
  int current_actions;

  // The number of scrap this character holds
  int num_scrap;

  // The self destruct tracker
  int self_destruct_tracker;

  // The number of items this character holds
  int num_items;
  // General held items (not coolant)
  item *held_items[3];
  // A held item that can only be a coolant
  item *coolant;

  // Pointer to the room this character is currently in
  room *current_room;

  // A text description of this character's ability
  char ability_description[128];
  // The pointer to this character's ability function
  ability_output *(*ability_function)(map *, character **, character *);
};

bool character_has_item(character *c, ITEM_TYPES type);

ability_output *ripley_ability(map *game_map, character *characters[5],
                               character *active_character);
ability_output *dallas_ability(map *game_map, character *characters[5],
                               character *active_character);
ability_output *parker_ability(map *game_map, character *characters[5],
                               character *active_character);
ability_output *brett_ability(map *game_map, character *characters[5],
                              character *active_character);
ability_output *lambert_ability(map *game_map, character *characters[5],
                                character *active_character);

extern character characters[5];

ability_output *new_ability_output();

void use_item(character *c, item *i);

void print_inventory(character *c);

#endif
