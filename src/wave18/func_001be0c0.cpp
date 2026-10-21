#include "game/scene_lifecycle.h"



void ExitSelectionScene::update()
{
    ExitSelectionScene *scene = this;
    if (func_00171e30() == 0)
        return;
    scene->prepare();
    func_001c20e8(scene->objects);
    scene->finish();
}
