/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map/map.h"
#include "map/room.h"
#include "utils.h"

room *get_room(map *game_map, const char *room_name)
{
    for (int i = 0; i < game_map->room_count; i++) {
        if (strcmp(game_map->rooms[i]->name, room_name) == 0) {
            return game_map->rooms[i];
        }
    }

    return NULL;
}

room *add_room_if_not_exists(char *room_name, map *new_map)
{
    int is_player_start = 0;
    int is_alien_start = 0;
    int is_ash_start = 0;
    if (room_name[0] == '&') {
        trim_string(room_name, -1);
        is_alien_start = 1;
    } else if (room_name[0] == '*') {
        trim_string(room_name, -1);
        is_player_start = 1;
    } else if (room_name[0] == '$') {
        trim_string(room_name, -1);
        is_ash_start = 1;
    } else if (room_name[0] == '%') {
        trim_string(room_name, -1);
    }

    room *already_exists_check = get_room(new_map, room_name);
    if (already_exists_check != NULL) {
        return already_exists_check;
    }

    room *new_room = create_room(room_name, 0);
    new_map->rooms[new_map->room_count] = new_room;
    new_map->room_count++;

    if (is_player_start) {
        new_map->player_start_room = new_room;
    } else if (is_alien_start) {
        new_map->xenomorph_start_room = new_room;
    } else if (is_ash_start) {
        new_map->ash_start_room = new_room;
    }

    return new_room;
}

map *read_map(const char *fn)
{
    // Allocate space for map
    map *new_map = (map *)malloc(sizeof(map));

    // Open map definition file
    FILE *fp;
    fp = fopen(fn, "r");
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] - Failed to open map file %s\n", fn);
        exit(1);
    }

    // Read map name
    fgets(new_map->name, 31, fp);
    strip_string(new_map->name, strlen(new_map->name));

    // Read rooms
    int fscanf_code;
    int scanning_scrap_events_coolant = 0;
    int scanning_ascii_map = 0;
    char line[256];

    new_map->player_start_room = NULL;
    new_map->xenomorph_start_room = NULL;
    new_map->ash_start_room = NULL;

    new_map->scrap_room_count = 0;
    new_map->event_room_count = 0;
    new_map->coolant_room_count = 0;

    while (fgets(line, 255, fp)) {
        if (line[0] == '~') {
            if (scanning_scrap_events_coolant == 0) {
                scanning_scrap_events_coolant = 1;
                continue;
            } else if (scanning_scrap_events_coolant > 0) {
                scanning_scrap_events_coolant = 4;
            } else if (scanning_ascii_map == 0) {
                scanning_ascii_map = 1;
                new_map->ascii_map = (char *)malloc(2048);
                new_map->ascii_map[0] = '\0';
                continue;
            } else if (scanning_ascii_map == 1) {
                scanning_ascii_map = 2;
            }
        }

        if (scanning_scrap_events_coolant > 0 && scanning_scrap_events_coolant < 4) {
            char *columns = strtok(line, ";");
            while (columns) {
                if (columns[0] == '\n') {
                    break;
                }

                room *found_room = get_room(new_map, columns);
                if (found_room == NULL) {
                    fprintf(stderr, "[ERROR] - Room %s not recognized\n", columns);
                    exit(1);
                }

                switch (scanning_scrap_events_coolant) {
                case 1:
                    new_map->scrap_rooms[new_map->scrap_room_count++] = found_room;
                    break;
                case 2:
                    new_map->event_rooms[new_map->event_room_count++] = found_room;
                    break;
                case 3:
                    new_map->coolant_rooms[new_map->coolant_room_count++] = found_room;
                    break;
                default:
                    fprintf(stderr, "[ERROR] - Something went wrong scanning for special rooms\n");
                    exit(1);
                    break;
                }

                columns = strtok(NULL, ";");
            }
            scanning_scrap_events_coolant++;
        } else if (scanning_ascii_map) {
            strcat(new_map->ascii_map, line);
            continue;
        }

        // Get line separated by semicolons
        char *columns = strtok(line, ";");
        int column_index = 0;

        room *target_room = NULL;

        // For each entry in the row
        while (columns) {
            // Strip trailing whitespace
            strip_string(columns, strlen(columns));

            if (column_index == 0) {
                if (is_letter(columns[0]) || columns[0] == '&' || columns[0] == '*' ||
                    columns[0] == '$') { // Named room
                    target_room = add_room_if_not_exists(columns, new_map);
                } else if (is_number(columns[0])) { // Chained corridors
                    char corridor_name[32] = "corridor_";
                    strcat(corridor_name, columns);

                    target_room = get_room(new_map, corridor_name);
                }
            } else {                         // Connections
                if (is_number(columns[0])) { // Corridors
                    char new_corridor_name[32] = "corridor_";
                    strcat(new_corridor_name, columns);

                    room *connected_corridor = add_room_if_not_exists(new_corridor_name, new_map);
                    connected_corridor->is_corridor = 1;

                    add_connection(target_room, connected_corridor);
                } else if (is_letter(columns[0])) { // Rooms
                    room *connected_room = get_room(new_map, columns);
                    add_connection(target_room, connected_room);
                } else if (columns[0] == '%') { // Ladder
                    room *ladder_room = add_room_if_not_exists(columns, new_map);

                    target_room->ladder_connection = ladder_room;
                    ladder_room->ladder_connection = target_room;
                }
            }

            // Move to next entry
            columns = strtok(NULL, ";");
            column_index++;
        }
    }

    if (new_map->player_start_room == NULL || new_map->xenomorph_start_room == NULL ||
        new_map->ash_start_room == NULL) {
        fprintf(stderr, "[ERROR] - Player, Xenomorph, and Ash start rooms must all be defined.");
        exit(1);
    }

    // Close map file
    fclose(fp);

    return new_map;
}

void print_map(const map *game_map)
{
    printf("---NAME---\n%s\n\n", game_map->name);

    printf("---ROOMS---\n");
    for (int i = 0; i < game_map->room_count; i++) {
        if (game_map->rooms[i]->is_corridor) {
            continue;
        }

        print_room(game_map->rooms[i], 1);

        if (game_map->player_start_room == game_map->rooms[i]) {
            printf("\t<Player Start Room>\n");
        } else if (game_map->xenomorph_start_room == game_map->rooms[i]) {
            printf("\t<Xenomorph Start Room>\n");
        } else if (game_map->ash_start_room == game_map->rooms[i]) {
            printf("\t<Ash Start Room>\n");
        }

        printf("\n");
    }
    printf("\n");

    printf("---CORRIDORS---\n");
    for (int i = 0; i < game_map->room_count; i++) {
        if (!game_map->rooms[i]->is_corridor) {
            continue;
        }

        printf("%s\n", game_map->rooms[i]->name);
    }
    printf("\n");
}
