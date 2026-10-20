#include "game/scene_runtime.h"
#include "game/functions.h"
#include "game/matrix.h"
#include "game/pool_list.h"

/* [INFERENCE] SceneNode retains its own observed prefix, not a DisplayPane or
 * LayoutPane alias; the common identity matrix is owned by matrix.h. */

extern "C" void func_001405f4(SceneNode *node)
{
    node->parent = 0;
    node->flags = 0;
    node->next = 0;
    func_00195334(&node->local, matrix34Identity());
    func_00195334(&node->world, matrix34Identity());
}
