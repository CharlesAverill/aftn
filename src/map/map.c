/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#include "map/map.h"

/**
 * Retrieves the room pointer corresponding to a given room name
 * @param  game_map                Map to look in
 * @param  room_name               Name of room to look for
 * @return           Pointer to room struct with name `room_name`
 */
room *get_room(map *game_map, const char *room_name)
{
    for (int i = 0; i < game_map->room_count; i++) {
        if (strcmp(game_map->rooms[i]->name, room_name) == 0) {
            return game_map->rooms[i];
        }
    }

    return NULL;
}

/**
 * Creates a new room with name `room_name` and adds it to `game_map` if another with the same
 * name does not exist
 * @param  room_name               Name of room to create
 * @param  game_map                Map to add room to
 * @return           Pointer to new room, or existing room if room with name `room_name` exists
 */
room *add_room_if_not_exists(map *game_map, char *room_name)
{
    bool is_player_start = false;
    bool is_alien_start = false;
    bool is_ash_start = false;
    if (room_name[0] == '&') {
        // Alien start room
        trim_string(room_name, -1);
        is_alien_start = true;
    } else if (room_name[0] == '*') {
        // Player start room
        trim_string(room_name, -1);
        is_player_start = true;
    } else if (room_name[0] == '$') {
        // Ash start room
        trim_string(room_name, -1);
        is_ash_start = true;
    } else if (room_name[0] == '%') {
        // Ladder connection
        trim_string(room_name, -1);
    }

    room *already_exists_check = get_room(game_map, room_name);
    if (already_exists_check != NULL) {
        return already_exists_check;
    }

    room *new_room = create_room(room_name, 0);
    game_map->rooms[game_map->room_count] = new_room;
    game_map->room_count++;

    if (is_player_start) {
        game_map->player_start_room = new_room;
    } else if (is_alien_start) {
        game_map->xenomorph_start_room = new_room;
    } else if (is_ash_start) {
        game_map->ash_start_room = new_room;
    }

    return new_room;
}

/**
 * Initialize a dfs_results struct
 * @return Pointer to an initialized dfs_results struct
 */
dfs_results *new_dfs_results()
{
    dfs_results *out = (dfs_results *)malloc(sizeof(dfs_results));
    out->num_results = 0;
    return out;
}

/**
 * Reset the search parameters of a map to prepare for a search
 * @param game_map  Map whose rooms' search parameters will be reset
 * @param distance  Distance to set room->search_distance to. Djikstra uses INT_MAX, DFS
 *                  uses -1
 */
void reset_search(map *game_map, int distance)
{
    for (int i = 0; i < game_map->room_count; i++) {
        game_map->rooms[i]->search_distance = distance;
        game_map->rooms[i]->search_discovered = false;
        game_map->rooms[i]->search_previous_room = NULL;
    }
}

/**
 * A modified DFS that finds rooms that are `distance` edges away
 * @param start_room  Room to start search from
 * @param distance    Distance at which to look for rooms
 */
void _find_rooms_by_distance_recurse(room *start_room, int distance)
{
    start_room->search_distance = distance;

    for (int i = 0; i < start_room->connection_count; i++) {
        if (start_room->connections[i]->search_distance == -1) {
            _find_rooms_by_distance_recurse(start_room->connections[i], distance + 1);
        }
    }

    if (start_room->ladder_connection != NULL &&
        start_room->ladder_connection->search_distance == -1) {
        _find_rooms_by_distance_recurse(start_room->ladder_connection, distance + 1);
    }
}

/**
 * Finds rooms within or exactly of a certain distance from a provided starting room
 * @param  game_map                 Map to search in
 * @param  start_room               Room to start searching from
 * @param  distance                 Distance to search up to or exactly to
 * @param  inclusive                If true, results will include rooms up to `distance` edges away
 * @return            A pointer to a dfs_results object of the matching rooms
 */
dfs_results *find_rooms_by_distance(map *game_map, room *start_room, int distance, bool inclusive)
{
    dfs_results *results = new_dfs_results();

    reset_search(game_map, -1);

    _find_rooms_by_distance_recurse(start_room, 0);

    for (int i = 0; i < game_map->room_count; i++) {
        if (game_map->rooms[i]->search_distance == distance ||
            (inclusive && game_map->rooms[i]->search_distance <= distance)) {
            results->rooms[results->num_results++] = game_map->rooms[i];
        }
    }

    reset_search(game_map, -1);

    return results;
}

/**
 * Reads in a map from a map file
 * @param  fn               Filename of map file
 * @return    Pointer to new map
 */
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

    new_map->room_count = 0;
    new_map->named_room_count = 0;

    new_map->ascii_map = "";

    while (fgets(line, 255, fp)) {
        // Section checking
        if (line[0] == '~') {
            if (scanning_scrap_events_coolant == 0) {
                scanning_scrap_events_coolant = 1;
                continue;
            } else if (scanning_scrap_events_coolant > 0 && scanning_scrap_events_coolant < 4) {
                scanning_scrap_events_coolant = 4;
                continue;
            } else if (scanning_ascii_map == 0) {
                scanning_ascii_map = 1;
                new_map->ascii_map = (char *)malloc(2048);
                new_map->ascii_map[0] = '\0';
                continue;
            } else if (scanning_ascii_map == 1) {
                scanning_ascii_map = 2;
                continue;
            }
        }

        // Scrap, Events, Coolant scanning
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
                // Named room
                if (is_letter(columns[0]) || columns[0] == '&' || columns[0] == '*' ||
                    columns[0] == '$') {
                    target_room = add_room_if_not_exists(new_map, columns);
                    new_map->named_room_indices[new_map->named_room_count++] =
                        new_map->room_count - 1;
                }
                // Chained corridors
                else if (is_number(columns[0])) {
                    char corridor_name[32] = "corridor_";
                    strcat(corridor_name, columns);

                    target_room = get_room(new_map, corridor_name);
                }
                // Connections
            } else {
                // Corridors
                if (is_number(columns[0])) {
                    char new_corridor_name[32] = "corridor_";
                    strcat(new_corridor_name, columns);

                    room *connected_corridor = add_room_if_not_exists(new_map, new_corridor_name);
                    connected_corridor->is_corridor = true;

                    add_connection(target_room, connected_corridor);
                    // Rooms
                } else if (is_letter(columns[0])) {
                    room *connected_room = get_room(new_map, columns);
                    add_connection(target_room, connected_room);
                    // Ladder
                } else if (columns[0] == '%') {
                    room *ladder_room = add_room_if_not_exists(new_map, columns);

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

/**
 * Print out a text representation of the map
 * @param game_map  The map to print out
 */
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
