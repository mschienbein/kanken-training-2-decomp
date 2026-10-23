#include "game/path_iterator.h"

unsigned func_001c5354(PathIterator *iterator, PathSlice *previous)
{
    unsigned index = 0;
    previous->length = iterator->end - iterator->begin;
    previous->text = iterator->begin;
    iterator->begin = iterator->next;
    for (;;) {
        unsigned short character = iterator->begin[index];
        /* ORIGINAL BUG: only slash-terminated components receive the length check. */
        if (character == '/') {
            if (index >= 256)
                return 0xe0e046c8;
            iterator->end = iterator->begin + index;
            iterator->next = iterator->end + 1;
            while (*iterator->next == '/')
                ++iterator->next;
            if (*iterator->next == 0)
                iterator->finished = true;
            return 0;
        }
        if (character == 0) {
            iterator->finished = true;
            iterator->end = iterator->next = iterator->begin + index;
            break;
        }
        ++index;
    }
    return 0;
}
