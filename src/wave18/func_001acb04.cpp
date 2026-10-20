#include "game/scene_lifecycle.h"



void MenuResetScene::update()
{
    MenuResetScene *scene = this;
    scene->prepare();
    func_001c20e8(scene->objects);
    scene->finish();
}
