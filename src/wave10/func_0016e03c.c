#include "game/character_panel.h"

static __forceinline int get_entry_value(const struct CharacterPanel *controller,
                                          int index)
{
    const struct WidgetSlot *entry = controller->slots[index];
    int value = 0;
    if (entry != 0 && entry->object != 0 && (entry->state.byte_flags[0] & 0x10))
        value = entry->text[0];
    return value;
}

int func_0016e03c(const struct CharacterPanel *controller, int index)
{
    return get_entry_value(controller, index + 11);
}
