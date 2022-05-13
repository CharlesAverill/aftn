/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arguments.h"
#include "manager.h"
#include "map/map.h"

const char *DEFAULT_MAP = "/var/games/aftn/game_data/maps/default";
const char *BANNER = "/var/games/aftn/game_data/banner.txt";

const char *argp_program_version = "aftn 0.0.1";
const char *argp_program_bug_address = "charles@utdallas.edu";
static char doc[] = "A C port of Alien: Fate of The Nostromo, a 2021 board game of the same name";
static char args_doc[] = "";

static struct argp_option options[] = {
    {"n_players", 'n', "integer", 0, "Number of players to create"},
    {"n_characters", 'c', "integer", 0, "Number of characters to create"},
    {"use_ash", 'a', 0, 0, "Include Ash for a more challenging game"},
    {"game",
     'g',
     "FILE",
     0,
     "Read game board from this path rather than the default. Check "
     "/var/games/aftn/maps/format.txt to create your own game boards"},
    {"print_map", 'p', 0, 0, "Print out a text representation of the game map"},
    {"draw_map", 'd', 0, 0, "Draw the game map if an ASCII map is provided"},
    {0}};

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[])
{
    // Argument parsing
    struct arguments arguments;
    arguments.n_players = 1;
    arguments.n_characters = 1;
    arguments.use_ash = 0;
    strcpy(arguments.game_file, DEFAULT_MAP);
    arguments.print_map = 0;
    arguments.draw_map = 0;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.n_players > 1) {
        fprintf(stderr, "[UNIMPLEMENTED] - Multiplayer is not yet implemented\n");
        exit(1);
    }

    // Print banner
    FILE *fp;
    fp = fopen(BANNER, "r");
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] - Could not open banner file %s\n", BANNER);
        exit(1);
    }

    char line[128];
    while (fgets(line, 127, fp)) {
        printf("%s", line);
    }

    fclose(fp);

    map *game_map = read_map(arguments.game_file);

    if (arguments.print_map) {
        print_map(game_map);
        exit(0);
    }

    if (arguments.draw_map) {
        printf("%s\n", game_map->ascii_map);
        exit(0);
    }

    // Create new game
    game_manager *manager = new_game(arguments, game_map);

    // Start game loop
    game_loop(manager);

    for (int i = 0; i < game_map->room_count; i++) {
        free(game_map->rooms[i]);
    }

    free(game_map->ascii_map);
    free(game_map);

    free(manager);

    return 0;
}
