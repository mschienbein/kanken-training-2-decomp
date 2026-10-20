#include "game/hwr_runtime.h"

/* [INFERENCE] Address-neighbourhood grouping is a translation-unit hypothesis,
 * not proof of the original source-file boundary. */

static unsigned short choose_direction(int major, int minor,
                                       unsigned short axial,
                                       unsigned short diagonal)
{
    return major > minor * 2 ? axial : diagonal;
}

static void penalize_group_duplicate(struct RecognitionList *list,
                                     unsigned short current,
                                     unsigned short other,
                                     unsigned short threshold)
{
    if (list->entries[current].candidate->next ==
        list->entries[other].candidate->next) {
        if (list->entries[current].score > list->entries[other].score)
            list->entries[current].score = threshold;
        else
            list->entries[other].score = threshold;
    }
}

static void trim_candidates(struct RecognitionList *list,
                            unsigned short threshold,
                            unsigned short minimum_count)
{
    unsigned short i;
    for (i = 0; i < list->count; ++i) {
        if (list->entries[i].score >= threshold && i >= minimum_count) {
            list->count = i;
            return;
        }
    }
}

static struct HwrCandidateStorage *candidate_storage(void)
{
    return (struct HwrCandidateStorage *)data_00243830;
}

unsigned short func_0015ad74(short x1, short y1, short x2, short y2)
{
    short dx = x2 - x1;
    short dy = y2 - y1;
    int negative_y;
    if (dx == 0 && dy == 0)
        return 16;
    if (dx >= 0) {
        if (dy > 0) {
            if (dy <= dx)
                return choose_direction(dx, dy, 15, 14);
            return choose_direction(dy, dx, 12, 13);
        }
        negative_y = -(short)(y2 - y1);
        if (negative_y < dx)
            return choose_direction(dx, negative_y, 0, 1);
        if (negative_y <= dx * 2)
            return 2;
        return dx > 0 ? 3 : 4;
    } else {
        int negative_x = -(short)(x2 - x1);
        if (dy >= 0) {
            if (dy < negative_x)
                return choose_direction(negative_x, dy, 8, 9);
            return choose_direction(dy, negative_x, 11, 10);
        }
        negative_y = -(short)(y2 - y1);
        if (negative_y <= negative_x)
            return choose_direction(negative_x, negative_y, 7, 6);
        return choose_direction(negative_y, negative_x, 4, 5);
    }
}

void func_0015b950(unsigned short stroke, int mode)
{
    unsigned short index = stroke - 1;
    int offset = 0;
    unsigned short i, j;
    unsigned short threshold;
    struct RecognitionList *candidates;
    const struct HwrStrokeLengths *input = &data_0024385c->lengths;
    const unsigned short *length = input->lengths;
    /* ORIGINAL BUG: a zero stroke number wraps the one-based index. */
    for (i = 0; i <= index; ++i)
        offset = (unsigned short)(offset + *length++);
    offset = (unsigned short)(offset + index);
    /* ORIGINAL BUG: zero accumulated length indexes one byte before the table. */
    threshold = data_00243864[offset - 1];
    if ((mode & 2) == 0)
        threshold = 127;
    else
        func_0017d8a8(index);
    candidates = &candidate_storage()->lists[index];
    candidates->entries = candidate_storage()->entries[index];
    candidates->capacity = 16;
    candidates->count = 0;
    candidates->worst_score = threshold;
    func_0015c5a4(candidates);
    func_0015c734(mode, 0);
    for (i = 1; i < candidates->count; ++i) {
        for (j = 0; j < i; ++j)
            penalize_group_duplicate(candidates, i, j, threshold);
    }
    if (candidates->count > 1)
        func_0017b4ac(candidates, 0, (short)(candidates->count - 1));
    trim_candidates(candidates, threshold, 2);
}
