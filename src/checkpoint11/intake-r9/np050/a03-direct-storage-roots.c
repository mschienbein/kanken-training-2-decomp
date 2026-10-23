#include "game/hwr_runtime.h"

/* [INFERENCE] Address-neighbourhood grouping is a translation-unit hypothesis,
 * not proof of the original source-file boundary. */


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
