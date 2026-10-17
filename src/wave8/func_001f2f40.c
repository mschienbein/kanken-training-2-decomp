#include "game/scene_lifecycle.h"

int func_001f2f40(const struct SceneEngine *engine)
{
    if (engine->inhibited)
        return 0;
    return engine->active.count != 0;
}
