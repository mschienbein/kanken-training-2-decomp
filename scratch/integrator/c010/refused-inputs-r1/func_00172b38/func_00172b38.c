#include "game/screen.h"

struct MaskedPanel13 {
    int columns;
    int rows;
    unsigned masks[1];
    struct DisplayHandle *handles[13];
};
extern const struct NameTable *bss_002471f8[];
extern struct DisplayObject *func_0017a818(const struct NameTable *table,
    int layer, int enabled, float x, float y, float z);

static __forceinline struct DisplayObject *create_grid_object(
    const struct NameTable *table, float depth)
{
    return func_0017a818(table, 4, 0, 0.0f, 0.0f, depth);
}

void func_00172b38(struct MaskedPanel13 *panel)
{
    int index = 0;
    int row;
    /* ORIGINAL BUG: further rows can read beyond the single mask and the
     * per-row limit does not prevent a later row from exceeding the handles. */
    for (row = 0; row < panel->rows; ++row) {
        unsigned column;
        for (column = 0; (int)column < panel->columns; ++column) {
            if (panel->masks[row] & (1u << column)) {
                struct DisplayHandle *handle = panel->handles[index];
                if (handle && !handle->object)
                    handle->object = create_grid_object(bss_002471f8[index], 1.2f);
            }
            if (++index >= 13)
                break;
        }
    }
}
