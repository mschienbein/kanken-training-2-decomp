#include "game/audio_voice.h"
#include "game/item_pages.h"
#include "game/command_state.h"
/* [INFERENCE] Shared page-set and content prefixes, not complete native classes.
 * The category is stored as a byte but forwarded as the original full word. */
void func_001719f4(struct ItemPageSet *state, unsigned char kind, int format,
                  int category, int index, int enabled)
{
    unsigned char complex_format;
    state->kind = kind;
    complex_format = func_001dc390(format);
    state->index = index;
    state->variant = complex_format;
    state->group = category;
    func_00171a34(state->content, category, index, enabled);
}
