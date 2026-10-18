#include "game/pane_descriptors.h"

struct SelectionCleanup_001b41b8 {
    int bits_per_word;
    int word_count;
    unsigned int masks[1];
    struct DisplayHandle *handles[10];
    unsigned char reset;
    unsigned char reserved35[0x43];
    struct MotionState motion;
};


extern void *func_0019ffa8(unsigned size);
extern void func_001852f8(void *allocation);
extern void func_00183208(struct DisplayObject *object);
extern void func_0017a938(const struct NameTable *const *descriptors, int count,
                         int option);
extern void func_0016f024(struct MotionState *state);

static __inline void release_handle(struct DisplayHandle *handle)
{
    if (handle && handle->object)
        func_00183208(handle->object);
}

static __inline void reset_handle(struct DisplayHandle *handle)
{
    if (handle) {
        handle->object = 0;
    }
}

void func_001b41b8(struct SelectionCleanup_001b41b8 *state)
{
    const struct NameTable **selected;
    int count;
    int source_index;
    int i, word, bit;

    for (i = 0; i < 10; ++i) {
        if (state->handles[i]) {
            release_handle(state->handles[i]);
            reset_handle(state->handles[i]);
        }
    }
    selected = func_0019ffa8(8);
    for (i = 0; i < 2; ++i)
        selected[i] = 0;
    count = 0;
    source_index = 0;
    for (word = 0; word < state->word_count; ++word) {
        for (bit = 0; bit < state->bits_per_word; ++bit) {
            if ((1u << bit) & state->masks[word])
                selected[count++] = data_0023ecc4[source_index];
            if (++source_index >= 1)
                break;
        }
    }
    if (count > 0) {
        selected[count] = data_0023ecc4[1];
        func_0017a938(selected, -1, 0);
    }
    for (i = 0; i < state->word_count; ++i)
        state->masks[i] = 0;
    func_0016f024(&state->motion);
    state->reset = 1;
    func_001852f8(selected);
}
