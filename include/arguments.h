/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <argp.h>
#include <stdlib.h>
#include <string.h>

typedef struct arguments {
    int n_players;
    int n_characters;

    int use_ash;

    int print_map;
    int draw_map;

    char game_file[256];
} arguments;

error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif
