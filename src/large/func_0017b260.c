#include "game/hwr_runtime.h"
void func_0017b260(struct RecognitionList *list, const struct RecognitionCandidate *candidate,
                   unsigned short score, unsigned short first,
                   unsigned short second, unsigned short flags, unsigned short mode)
{
    unsigned short i;
    struct RecognitionEntry *entries = list->entries;
    if (mode == 2) {
        if (candidate) {
            first = candidate->character;
            if (first == 0)
                return;
        }
        for (i = 0; i < list->count; ++i) {
            if (entries[i].candidate == candidate) {
                if (entries[i].score > score) {
                    entries[i].score = score;
                    goto check_worst;
                }
                return;
            }
        }
    } else if (mode != 0) {
        for (i = 0; i < list->count; ++i) {
            if (entries[i].candidate == candidate && entries[i].first == first &&
                entries[i].second == second)
                return;
        }
    }
    if (list->count < list->capacity) {
        entries[list->count].candidate = candidate;
        entries[list->count].score = score;
        entries[list->count].first = first;
        entries[list->count].second = second;
        entries[list->count].flags = flags;
        ++list->count;
    } else {
        /* ORIGINAL BUG: zero capacity is not checked before replacing worst_index. */
        entries[list->worst_index].candidate = candidate;
        entries[list->worst_index].score = score;
        entries[list->worst_index].first = first;
        entries[list->worst_index].second = second;
        entries[list->worst_index].flags = flags;
    }
check_worst:
    if (list->count == list->capacity) {
        list->worst_score = 0;
        for (i = 0; i < list->capacity; ++i) {
            if (list->worst_score < entries[i].score) {
                list->worst_score = entries[i].score;
                list->worst_index = i;
            }
        }
    }
}
