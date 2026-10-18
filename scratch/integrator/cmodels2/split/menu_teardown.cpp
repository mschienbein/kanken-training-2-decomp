#include "game/menu_resources.h"

/* [INFERENCE] Isolated cleanup unit from the inherited UI-resource hypothesis.
 * The native point/rectangle lifetime identities and historical TU membership
 * remain unrecovered. Only func_001e958c is emitted; unrelated matched/mismatching
 * context definitions are not delivered or credited. */
namespace MenuResources {
static Panel *panel_at(State *menu, int index);
static void clear_panel(State *menu, int index);
}

void func_001e958c(MenuResources::State *menu)
{
    for (int i = 0; i < 20; ++i)
        MenuResources::clear_panel(menu, i);
}

namespace MenuResources {

static void clear_panel(State *menu, int index)
{
    Panel *panel = panel_at(menu, index);
    if (panel) {
        panel->hovered = -1;
        if (panel->items) {
            for (int j = 0; j < panel->count; ++j) {
                Item *item = panel->items[j];
                if (item)
                    delete item;
            }
            func_0019e08c(panel->items);
        }
        func_0016f024(&panel->motion);
        func_001e9ea4(menu, index);
    }
}

inline Item::~Item()
{
    if (object)
        func_00183208(object);
    delete hit_bounds;
    object = 0;
    table = 0;
    identifier = -1;
    behavior = 0;
    flags = 0;
    countdown = 0;
    initial = 0;
    current = 0;
    state = 0;
    position.x = 0.0f;
    position.y = 0.0f;
    size.x = 0.0f;
    size.y = 0.0f;
    value = 0.0f;
    parameter = 0;
    hit_bounds = 0;
}

static Panel *panel_at(State *menu, int index)
{
    return menu->panels[index];
}

}
