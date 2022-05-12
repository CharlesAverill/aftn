/**
 * @file
 * @author Charles Averill
 * @date   12-May-2022
 * @brief Description
*/

#include "manager.h"

#include <stdlib.h>

#include "character.h"
#include "map/map.h"

game_manager *new_game(const arguments args, map *game_map)
{
    game_manager *manager = (game_manager *)malloc(sizeof(game_manager));

    manager->morale = args.n_players > 3 ? 20 : 15;

    manager->game_map = game_map;
    manager->xenomorph_location = manager->game_map->xenomorph_start_room;
    manager->ash_location = manager->game_map->ash_start_room;

    for (int i = 0; i < manager->game_map->scrap_room_count; i++) {
        manager->game_map->scrap_rooms[i]->num_scrap = 2;
    }
    for (int i = 0; i < manager->game_map->event_room_count; i++) {
        manager->game_map->scrap_rooms[i]->has_event = 1;
    }
    for (int i = 0; i < manager->game_map->coolant_room_count; i++) {
        //manager->game_map->scrap_rooms[i]->num_scrap = 2;
    }

    manager->round_index = 1;
    manager->turn_index = 0;

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
    }

    return manager;
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
                    switch (ch) {
                    case 'h':
                        printf("m - move\np - pick up\nd - drop\na - ability\nc - craft\ni - use "
                               "item\nt - trade\n");
                        break;
                    case 'm':
                        printf("Move\n");
                        break_loop = 1;
                        break;
                    case 'p':
                        printf("Pick up\n");
                        break_loop = 1;
                        break;
                    case 'd':
                        printf("Drop\n");
                        break_loop = 1;
                        break;
                    case 'a':
                        printf("Use ability\n");
                        break_loop = 1;
                        break;
                    case 'c':
                        printf("Craft\n");
                        break_loop = 1;
                        break;
                    case 'i':
                        printf("Use item\n");
                        break_loop = 1;
                        break;
                    case 't':
                        printf("Trade\n");
                        break_loop = 1;
                        break;
                    }

                    if (break_loop) {
                        break;
                    } else {
                        printf("Unrecognized command\n");
                    }
                }
            }
        }

        manager->round_index++;
    }
}
