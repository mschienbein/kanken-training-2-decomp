#include "game/render_state.h"

extern struct SlotList *data_002426d8;

int func_00118074(struct SlotList *list)
{
    struct SlotBlock *block;
    int result;
    data_002426d8 = list;
    block = func_0018ce64();
    if (block) {
        block->next = 0;
        block->previous = 0;
        data_002426d8->head = block;
        data_002426d8->head_count = 0;
        data_002426d8->tail = block;
        data_002426d8->tail_count = 0;
        result = 0;
    } else {
        result = -1;
    }
    return result;
}
