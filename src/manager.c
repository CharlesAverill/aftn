/**
 * @file
 * @author Charles Averill
 * @date   12-May-2022
 * @brief Description
*/

#include "manager.h"

#include <stdlib.h>
#include <time.h>

#include "character.h"
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
    manager->ash_location = manager->game_map->ash_start_room;

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

        char ch = 0;
        while (ch < '0' || ch > '5') {
            ch = getc(stdin);
        }

        int selection = ch - '0' - 1;
        picked[i] = selection;
        manager->characters[i] = &characters[selection];

        manager->characters[i]->current_room = manager->game_map->player_start_room;
    }
    manager->character_count = args.n_characters;

    // Set random seed
    srand(time(0));

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
    if (1 || moved->current_room->has_event) {
        int event_type = randint(1, 12);
        event_type = 12;

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

            reduce_morale(manager, lost_morale);

            dfs_results *allowed_moves =
                find_rooms_by_distance(manager->game_map, moved->current_room, 3, 0);
            moved->current_room = character_move(manager, moved, allowed_moves, 0);

            return 2;
        }
    }

    return 0;
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
                   manager->turn_index,
                   manager->active_character->last_name);

            printf("h - view help menu\n");

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
                        printf("m - move\np - pick up\nd - drop\na - ability\nc - craft\ni - use "
                               "item\nt - trade\nq - draw map\nr - print text map\ne - exit\n");
                        recognized = 1;
                        break;
                    case 'm':
                        printf("Move\n");
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
                            trigger_event(manager, manager->active_character);
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
        }

        manager->round_index++;
    }
}
