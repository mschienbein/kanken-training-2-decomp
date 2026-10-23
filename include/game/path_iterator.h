#ifndef GAME_PATH_ITERATOR_H
#define GAME_PATH_ITERATOR_H

#include <stdbool.h>
/* Hash-bound incremental author witness; descriptive field/type names remain
 * [INFERENCE]. Shared declarations replace all local copies in collected units. */
struct PathSlice {
    int length;
    const unsigned short *text;
};
struct PathIterator {
    const unsigned short *begin;
    const unsigned short *end;
    const unsigned short *next;
    bool finished;
};

#ifdef __cplusplus
extern "C" {
#endif
unsigned func_001c5354(struct PathIterator *iterator, struct PathSlice *previous);
#ifdef __cplusplus
}
#endif

#endif
