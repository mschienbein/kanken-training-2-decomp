#include "game/input.h"

bool func_00178fd4(const struct InputOwner *owner, int index)
{
    const struct InputGroup *group = owner->groups[index];
    int result = 0;
    if (group) {
        if (func_0016ec14(&group->mode))
            return result;
        return 1;
    }
    return result;
}
