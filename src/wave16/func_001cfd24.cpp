#include "game/matrix.h"
#include "game/pool_list.h"
#include "game/scene_runtime.h"



struct SceneNode *func_001cfd24(struct SceneNode *node, const struct SceneRecord *record)
{
    node = func_0016b120(node, record);
    node->vtable = data_002243f8;
    return node;
}
