#include "game/resource_font.h"
#include "game/pool_list.h"



struct PoolLink *func_001cfd9c(struct AnimationMaterial *material,
                                struct AnimationBinding *binding)
{
    return func_00191154(&material->animations, &material->animations.sentinel,
                          &binding->link);
}
