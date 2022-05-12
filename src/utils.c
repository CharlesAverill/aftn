/**
 * @file
 * @author Charles Averill
 * @date   11-May-2022
 * @brief Description
*/

#include "utils.h"

#include <string.h>
#include <strings.h>

void strip_string(char *str, int len)
{
    int index = len - 1;

    while (str[index] == ' ' || str[index] == '\n' || str[index] == '\t') {
        index--;
    }

    str[index + 1] = '\0';
}

void trim_string(char *str, int size)
{
    if (size < 0) { // trim abs(size) from front of string
        memmove(str, str + (size * -1), strlen(str));
    } else { // trim abs(size) from end of string
        int len = strlen(str);
        str[len - size] = '\0';
    }
}

int is_letter(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_number(char c)
{
    return (c >= '0' && c <= '9');
}
