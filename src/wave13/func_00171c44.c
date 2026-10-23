#include "game/drawing.h"
#include "game/selection.h"

int func_00171c44(const struct W6ListController *controller, int index)
{
    const struct W6ListGroup *group = controller->lists[index];
    return group ? group->count : 0;
}
