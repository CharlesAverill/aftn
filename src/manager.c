/**
 * @file
 * @author Charles Averill
 * @date   12-May-2022
 * @brief Description
*/

#include "manager.h"

#include <limits.h>
#include <stdlib.h>
#include <time.h>

#include "character.h"
#include "encounter.h"
#include "map/map.h"

game_manager *new_game(const arguments args, map *game_map)
{
    game_manager *manager = (game_manager *)malloc(sizeof(game_manager));

    // Team morale
    manager->morale = args.n_players > 3 ? 20 : 15;

    // Map setup
    manager->game_map = game_map;

    // Initialize Xenomorph and Ash locations
    manager->xenomorph_location = manager->game_map->xenomorph_start_room;
    if (args.use_ash) {
        manager->ash_location = manager->game_map->ash_start_room;
    } else {
        manager->ash_location = NULL;
    }

    // Place initial scrap
    for (int i = 0; i < manager->game_map->scrap_room_count; i++) {
        manager->game_map->scrap_rooms[i]->num_scrap = 2;
    }

    // Place initial events
    for (int i = 0; i < manager->game_map->event_room_count; i++) {
        manager->game_map->scrap_rooms[i]->has_event = 1;
    }

    // Place initial coolant
    for (int i = 0; i < manager->game_map->coolant_room_count; i++) {
        //manager->game_map->scrap_rooms[i]->num_scrap = 2;
    }

    // Game setup
    manager->round_index = 1;
    manager->turn_index = 0;

    // Character selection
    int picked[5] = {-1, -1, -1, -1, -1};
    for (int i = 0; i < 5; i++) {
        manager->characters[i] = NULL;
    }
    for (int i = 0; i < args.n_characters; i++) {
        printf("Pick character %d:\n", i + 1);

        for (int j = 0; j < 5; j++) {
            int already_picked = 0;
            for (int k = 0; k < 5; k++) {
                if (picked[k] == j) {
                    already_picked = 1;
                }
            }
            if (already_picked) {
                continue;
            }

            printf("%d) %s, %s - %d Actions - Special Ability: %s\n",
                   j + 1,
                   characters[j].last_name,
                   characters[j].first_name,
                   characters[j].max_actions,
                   characters[j].ability_description);
        }
        printf("e) Exit\n");

        char ch = 0;
        while (ch < '0' || ch > '5') {
            ch = getchar();
            if (ch == 'e') {
                exit(0);
            }
        }

        int selection = ch - '0' - 1;
        picked[i] = selection;
        manager->characters[i] = &characters[selection];

        manager->characters[i]->current_room = manager->game_map->player_start_room;
    }
    manager->character_count = args.n_characters;

    // Set random seed
    srand(time(0));

    // Shuffle encounter deck
    shuffle_encounters();

    return manager;
}

int randint(int low, int high)
{
    return (rand() % (high - low + 1)) + low;
}

room *character_move(game_manager *manager,
                     struct character *to_move,
                     dfs_results *allowed_moves,
                     int allow_back)
{
    // Get selection
    char ch = '\0';
    while (1) {
        // Print choices
        printf("Destinations:\n");
        if (allowed_moves == NULL) {
            // Move to adjacent rooms
            for (int i = 0; i < to_move->current_room->connection_count; i++) {
                printf("\t%d) %s\n", i + 1, to_move->current_room->connections[i]->name);
            }
            if (to_move->current_room->ladder_connection != NULL) {
                printf("\tl) Ladder to %s\n", to_move->current_room->ladder_connection->name);
            }
        } else {
            // Move to rooms defined in allowed_moves
            for (int i = 0; i < allowed_moves->num_results; i++) {
                printf("\t%d) %s\n", i + 1, allowed_moves->rooms[i]->name);
            }
        }
        if (allow_back) {
            printf("\tb) Back\n");
        }

        // Get input
        getchar();
        ch = getchar();

        int max_destination_index = allowed_moves == NULL ? to_move->current_room->connection_count
                                                          : allowed_moves->num_results;
        if (allow_back && ch == 'b') {
            return to_move->current_room;
        } else if (allowed_moves == NULL && ch == 'l') {
            return to_move->current_room->ladder_connection;
        } else if (ch >= '0' && ch <= max_destination_index + '0') {
            if (allowed_moves == NULL) {
                return to_move->current_room->connections[ch - '0' - 1];
            } else {
                return allowed_moves->rooms[ch - '0' - 1];
            }
        }
    }

    return NULL;
}

struct room_queue *shortest_path(map *game_map, room *source, room *target)
{
    // Set all to unvisited and distance to inf
    reset_search(game_map, INT_MAX);
    // Set start vertex (xeno room) distance to 0
    source->search_distance = 0;

    // Create queue of rooms
    struct room_queue *rq = newQueue(64);
    for (int i = 0; i < game_map->room_count; i++) {
        push(rq, game_map->rooms[i]);
    }

    while (rq->head != NULL) {
        // Get closest node
        int min_node_distance = INT_MAX;
        room *min_node = rq->head;
        room *tmp = rq->head;
        while (tmp != NULL) {
            if (tmp->search_distance < min_node_distance) {
                min_node_distance = tmp->search_distance;
                min_node = tmp;
            }
            tmp = tmp->room_queue_next;
        }

        if (min_node == target) {
            struct room_queue *shortest_path = newQueue(64);
            if (min_node->search_previous_room != NULL || min_node == source) {
                while (min_node != NULL) {
                    push(shortest_path, min_node);
                    min_node = min_node->search_previous_room;
                }

                return shortest_path;
            }
        }

        // Remove closest node from rq
        if (min_node == rq->head) {
            rq->head = rq->head->room_queue_next;
        } else {
            room *traverse = rq->head;
            while (traverse->room_queue_next != min_node) {
                traverse = traverse->room_queue_next;
            }
            traverse->room_queue_next = min_node->room_queue_next;
        }

        // For each connection of min_node in rq
        if (min_node != NULL) {
            for (int i = 0; i < min_node->connection_count; i++) {
                room *neighbor = min_node->connections[i];
                if (queue_contains(rq, neighbor)) {
                    int alt = min_node->search_distance + 1;
                    if (alt < neighbor->search_distance) {
                        neighbor->search_distance = alt;
                        neighbor->search_previous_room = min_node;
                    }
                }
            }

            if (queue_contains(rq, min_node->ladder_connection)) {
                int alt = min_node->search_distance + 1;
                if (alt < min_node->ladder_connection->search_distance) {
                    min_node->ladder_connection->search_distance = alt;
                    min_node->ladder_connection->search_previous_room = min_node;
                }
            }
        }
    }

    return NULL;
}

// Modified Djikstra
room *xeno_move(game_manager *manager, int num_spaces, int morale_drop)
{
    struct room_queue *shortest = NULL;
    int s = INT_MAX;
    for (int i = 0; i < manager->character_count; i++) {
        struct room_queue *rq = shortest_path(
            manager->game_map, manager->xenomorph_location, manager->characters[i]->current_room);

        if (rq != NULL && rq->size < s) {
            shortest = rq;
            s = rq->size;
        }
    }

    if (s < num_spaces) {
        manager->xenomorph_location = shortest->tail;
    } else {
        for (int i = 0; i < num_spaces && shortest->head->room_queue_next != NULL; i++) {
            pop_tail(shortest);
        }
        manager->xenomorph_location = shortest->tail;
    }

    int printed_message = 0;
    for (int i = 0; i < manager->character_count; i++) {
        if (manager->characters[i]->current_room == manager->xenomorph_location) {
            if (!printed_message) {
                printed_message = 1;
                printf("The Xenomorph meets you in %s!\n", manager->xenomorph_location);
            }
            flee(manager, manager->characters[i]);
        }
    }
}

room *ash_move(game_manager *manager, int num_spaces)
{
}

void reduce_morale(game_manager *manager, int lost)
{
    for (int i = 0; i < manager->character_count; i++) {
        // TODO - Check if any character has electric prod to reduce morale deducted
    }

    manager->morale -= lost;

    if (manager->morale <= 0) {
        printf("[GAME OVER] - Morale dropped to 0\n");
        exit(0);
    }
}

int trigger_event(game_manager *manager, struct character *moved)
{
    if (moved->current_room->has_event) {
        int event_type = randint(1, 12);

        if (event_type <= 8) {
            printf("[EVENT] - Safe\n");

            return 0;
        } else if (event_type <= 10) {
            printf("[EVENT] - Jonesy\n");
            printf("Jonesy hisses at you! Morale decreases by 1.\n");

            reduce_morale(manager, 1);

            return 1;
        } else {
            printf("[EVENT] - Surprise Attack\n");
            int lost_morale = randint(1, 2);
            printf(
                "You encounter the Xenomorph! Morale decreases by %d and you must flee 3 spaces.\n",
                lost_morale);

            manager->xenomorph_location = moved->current_room;

            reduce_morale(manager, lost_morale);

            flee(manager, moved);

            return 2;
        }
    }

    return 0;
}

int trigger_encounter(game_manager *manager)
{
    int discard_index = draw_encounter();
    ENCOUNTER_TYPES encounter = discard_encounters[discard_index];

    switch (encounter) {
    case QUIET:;
        room *target_room = manager->game_map->rooms[manager->game_map->named_room_indices[randint(
            0, manager->game_map->named_room_count - 1)]];
        printf("[ENCOUNTER] - All is quiet in %s. Alien moves 1 space.\n", target_room->name);

        int scrap_decider = randint(1, 11);
        if (scrap_decider <= 8) {
            target_room->num_scrap += 2;
        } else if (scrap_decider <= 10) {
            target_room->num_scrap += 3;
        } else {
            target_room->num_scrap += 1;
        }

        target_room->has_event = 1;

        xeno_move(manager, 1, 2);

        break;
    case ALIEN_Lost_The_Signal:
        printf("[ENCOUNTER] - We lost the signal. Xenomorph has returned to %s\n",
               manager->game_map->xenomorph_start_room->name);

        manager->xenomorph_location = manager->game_map->xenomorph_start_room;
        //xeno_move(manager, 0, 2);
        replace_alien_cards();

        break;
    case ALIEN_Stalk:
        printf("[ENCOUNTER] - The Xenomorph is stalking...\n");
        xeno_move(manager, 3, 3);

        break;
    case ALIEN_Hunt:
        printf("[ENCOUNTER] - The Xenomorph is hunting!\n");
        xeno_move(manager, 2, 4);

        break;
    case ORDER937_Meet_Me_In_The_Infirmary:
        printf("[ENCOUNTER] - Ash moves twice, and %s moves to %s\n",
               manager->active_character->last_name,
               manager->game_map->ash_start_room->name);
        manager->active_character->current_room = manager->game_map->ash_start_room;
        ash_move(manager, 2);

        break;
    case ORDER937_Crew_Expendable:
        printf("[ENCOUNTER] - Crew Expendable - Ash moves twice, and %s loses all Scrap\n",
               manager->active_character->last_name);
        replace_order937_cards();
        ash_move(manager, 2);

        break;
    case ORDER937_Collating_Data:
        printf("[ENCOUNTER] - Collating data - Ash moves twice, and each character loses 1 "
               "Scrap\n");
        for (int i = 0; i < manager->character_count; i++) {
            manager->characters[i]->n_scrap = max(0, manager->characters[i]->n_scrap - 1);
        }
        ash_move(manager, 2);

        break;
    }
}

void flee(game_manager *manager, struct character *moved)
{
    printf("%s must flee 3 spaces:\n", moved->last_name);

    dfs_results *allowed_moves =
        find_rooms_by_distance(manager->game_map, moved->current_room, 3, 0);
    moved->current_room = character_move(manager, moved, allowed_moves, 0);
}

void game_loop(game_manager *manager)
{
    while (1) {
        printf("-----Round %d-----\n", manager->round_index);

        for (int i = 0; i < 5; i++) {
            if (manager->characters[i] == NULL) {
                break;
            }

            manager->turn_index = i;

            manager->active_character = manager->characters[manager->turn_index];
            printf("-----Turn %d: %s------\n",
                   manager->turn_index + 1,
                   manager->active_character->last_name);

            printf("h - view help menu\n");

            int do_encounter = 1;
            for (int j = manager->active_character->max_actions; j > 0; j--) {
                manager->active_character->current_actions = j;

                char ch;
                while (1) {

                    printf("Actions - %d/%d\n",
                           manager->active_character->current_actions,
                           manager->active_character->max_actions);

                    getchar();
                    ch = getchar();

                    int break_loop = 0;
                    int recognized = 0;
                    switch (ch) {
                    case 'h':
                        printf("m - move\n"
                               "p - pick up\n"
                               "d - drop\n"
                               "a - ability\n"
                               "c - craft\n"
                               "i - use item\n"
                               "t - trade\n"
                               "v - view current room\n"
                               "l - character locations\n"
                               "q - draw map\n"
                               "r - print text map\n"
                               "e - exit\n");

                        recognized = 1;
                        break;
                    case 'm':; // Start case with assignment
                        room *last_room = manager->active_character->current_room;
                        manager->active_character->current_room =
                            character_move(manager, manager->active_character, NULL, 1);
                        if (manager->active_character->current_room == last_room) {
                            printf("Canceled move\n");
                        } else {
                            printf("Moved %s from %s to %s\n",
                                   manager->active_character->last_name,
                                   last_room->name,
                                   manager->active_character->current_room->name);
                            if (trigger_event(manager, manager->active_character) ==
                                2) { // Alien encounter
                                // Immediately end turn and don't do an encounter
                                j = 0;
                                do_encounter = 0;
                            }
                            break_loop = 1;
                        }

                        recognized = 1;
                        break;
                    case 'p':
                        printf("Pick up\n");

                        break_loop = 1;
                        recognized = 1;
                        break;
                    case 'd':
                        printf("Drop\n");

                        break_loop = 1;
                        recognized = 1;
                        break;
                    case 'a':
                        printf("Use ability\n");

                        break_loop = 1;
                        recognized = 1;
                        break;
                    case 'c':
                        printf("Craft\n");

                        break_loop = 1;
                        recognized = 1;
                        break;
                    case 'i':
                        printf("Use item\n");

                        break_loop = 1;
                        recognized = 1;
                        break;
                    case 't':
                        printf("Trade\n");

                        break_loop = 1;
                        recognized = 1;
                        break;
                    case 'v':
                        print_room(manager->active_character->current_room, 1);

                        recognized = 1;
                        break;
                    case 'l':
                        for (int i = 0; i < manager->character_count; i++) {
                            printf("%s at %s\n",
                                   manager->characters[i]->last_name,
                                   manager->characters[i]->current_room->name);
                        }
                        printf("Xenomorph at %s\n", manager->xenomorph_location->name);
                        if (manager->ash_location != NULL) {
                            printf("Ash at %s\n", manager->ash_location->name);
                        }

                        recognized = 1;
                        break;
                    case 'q':
                        printf("%s\n", manager->game_map->ascii_map);

                        recognized = 1;
                        break;
                    case 'r':
                        print_map(manager->game_map);

                        recognized = 1;
                        break;
                    case 'e':
                        printf("Are you sure you want to exit? Game progress will not be saved. "
                               "(y/n)\n");
                        getchar();
                        if (getchar() == 'y') {
                            exit(0);
                        }

                        recognized = 1;
                    }

                    if (break_loop) {
                        break;
                    } else if (!recognized) {
                        printf("Unrecognized command\n");
                    }
                }
            }

            if (do_encounter) {
                trigger_encounter(manager);
            }
        }

        manager->round_index++;
    }
}
