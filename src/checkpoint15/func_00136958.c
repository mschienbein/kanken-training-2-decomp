#include <string.h>

/* [INFERENCE] Private operation boundaries and encoded output-unit width.
 * The observed caller uses width 2; the count does not depend on output. */
static int newline_count(unsigned newline)
{
    switch (newline) {
    case 1: return 2;
    case 2: case 3: return 1;
    default: return 0;
    }
}

static int write_ending(char *destination, unsigned width, unsigned newline)
{
    /* ORIGINAL BUG: a zero width underflows the clear length and last-byte index. */
    memset(destination, 0, width - 1);
    switch (newline) {
    case 1:
        destination[width - 1] = '\r';
        memset(destination + width, 0, width - 1);
        destination[2 * width - 1] = '\n';
        return 2;
    case 2:
        destination[width - 1] = '\r';
        return 1;
    case 3:
        destination[width - 1] = '\n';
        return 1;
    default:
        return 0;
    }
}

int func_00136958(char *destination, unsigned width, unsigned newline, unsigned write)
{
    if (write)
        return write_ending(destination, width, newline);
    return newline_count(newline);
}
