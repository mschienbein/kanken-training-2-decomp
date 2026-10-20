#include "game/input.h"
#include "game/screen.h"
#include "game/playback_screen.h"

int func_001e9af8(struct W22UiItem *item, const struct InputSnapshot *input)
{
    int result = 0;
    if (!(item->flags & 0x19)) {
        if (item->flags & 4) {
            result = 2;
            item->state = result;
        } else if (item->flags & 2) {
            result = 1;
            if (input->released) {
                if (item->countdown > 0) {
                    int mode = 1;
                    --item->countdown;
                    if (item->countdown <= 0) {
                        unsigned parameter = item->parameter;
                        item->flags |= 4;
                        if (!parameter)
                            parameter = 0x01000016;
                        mode = 0;
                        func_0015e31c(~0u, parameter);
                    }
                    if (item->current < item->table->count - 1) {
                        ++item->current;
                        if (!item->table->names[item->current]) {
                            do {
                                ++item->current;
                                if (item->current >= item->table->count) {
                                    item->current = 0;
                                    break;
                                }
                            } while (!item->table->names[item->current]);
                        }
                        func_0017a968(item->object, item->table, item->current, mode);
                    }
                    item->state = result;
                    item->flags &= ~2u;
                }
            } else if (!func_001703c4(item->hit_bounds, input->position.x, input->position.y)) {
                item->countdown = 0;
                item->flags &= ~6u;
                item->current = item->initial;
                result = -1;
                func_0017a968(item->object, item->table, item->current, 1);
                item->state = 0;
            }
        } else if (input->active == 1 && !input->previous_active) {
            if (func_001703c4(item->hit_bounds, input->position.x, input->position.y)) {
                item->flags |= 2;
                if (item->countdown <= 0) {
                    if (item->behavior == 0)
                        item->countdown = 1;
                    else if (item->behavior == 1) {
                        item->countdown = 2;
                        func_0015e31c(~0u, 0x01000015);
                    }
                }
            }
        }
    }
    return result;
}
