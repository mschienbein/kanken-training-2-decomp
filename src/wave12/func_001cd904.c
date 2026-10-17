#include "game/resource_font.h"
#include "game/pool_list.h"



struct PoolLink *func_001cd904(struct AnimationPane *pane,
                                struct AnimationBinding *binding)
{
    return func_00191154(&pane->animations, &pane->animations.sentinel,
                          &binding->link);
}
