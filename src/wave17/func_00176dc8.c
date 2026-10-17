struct W17SceneEntries {
    const void *entries[21];
};
struct W17SceneSelection {
    unsigned char reserved00[0x0c];
    int index;
    unsigned int pending;
    const void *selected;
    unsigned char reserved18[0x0c];
    struct W17SceneEntries table;
};
extern struct W17SceneSelection data_0023ee9c;

static const void *scene_entry_at(const struct W17SceneEntries *table, int index)
{
    return table->entries[index];
}

/* Direct non-tail callers ignore the result; the index is a signed word.
 * [INFERENCE] The selection and entry type names. */
void func_00176dc8(int index)
{
    /* ORIGINAL BUG: a negative index passes the upper-bound-only check. */
    if (index < 21)
        data_0023ee9c.selected = scene_entry_at(&data_0023ee9c.table, index);
    data_0023ee9c.index = index;
    data_0023ee9c.pending = 0;
}
