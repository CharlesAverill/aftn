/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#include "arguments.h"

/**
 * argp function to parse options
 */
error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key) {
    case 'n':
        arguments->n_players = atoi(arg);
        break;
    case 'c':
        arguments->n_characters = atoi(arg);
        break;
    case 'a':
        arguments->use_ash = 1;
        break;
    case 'g':
        strcpy(arguments->game_file, arg);
        break;
    case 'p':
        arguments->print_map = 1;
        break;
    case 'd':
        arguments->draw_map = 1;
        break;
    case ARGP_KEY_ARG:
        // Too many arguments, if your program expects only one argument.
        if (state->arg_num > 0)
            argp_usage(state);
        break;
    case ARGP_KEY_END:
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}
