#include "game/training_screen.h"
#include "game/item_pages.h"
#include "game/item_text.h"

void func_00169018(struct L041InitializeScreen *state, int enabled)
{
    if (state->scroll_shown) {
        if (enabled) {
            struct ItemPage *page = func_0017a4e8(data_0023ecf4, state->page);
            struct MenuItem *item = func_0017a49c(page, state->item, 0);
            /* [INFERENCE] Reuse the existing typed signed-byte accessor.
             * The original property load does not establish a helper call,
             * historical identity, outlining or translation-unit membership. */
            if (item != 0 && r41279_item_category(item) == 19) {
                struct WidgetPosition position;
                func_001ad6d0(state->message, 0, -1);
                position.x = -8.0f;
                position.y = 24.0f;
                func_001ad6ec(state->message, position);
            }
        }
        func_00177b84(state->message, enabled);
    }
}
