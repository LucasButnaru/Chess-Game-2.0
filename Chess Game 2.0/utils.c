#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

bool parse_position(char* pos, int* x, int* y) {
    if (strlen(pos) != 2 || pos[0] < 'a' || pos[0] > 'h' || pos[1] < '1' || pos[1] > '8') return false;
    *x = pos[0] - 'a';
    *y = 8 - (pos[1] - '0');
    return true;
}
