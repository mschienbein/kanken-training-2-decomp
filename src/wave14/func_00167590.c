#include "game/selection.h"
int func_00167590(int category, int member)
{
    int group, index;
    const struct W6Category *entry = &bss_00290120[category];
    for (group = 0; group < entry->group_count; ++group)
        for (index = 0; index < entry->patterns[group].value; ++index)
            if (entry->patterns[group].details.members[index] == member)
                return entry->patterns[group].selection;
    return -1;
}
