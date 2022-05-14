/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief The argument structure and accompanying function headers
*/

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <argp.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold command line arguments
typedef struct arguments {
    // Number of players (TODO : unimplemented)
    int n_players;
    // Number of characters
    int n_characters;

    // Whether or not to use Ash for a more challenging game
    bool use_ash;

    // Whether or not to print a text version of the map, then exit
    bool print_map;
    // Whether or not to print an ASCII drawing of the map, then exit
    bool draw_map;

    // A path to a map file
    char game_file[256];
} arguments;

error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif
