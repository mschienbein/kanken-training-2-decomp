#include "game/menu_controller.h"



void InitializationMenuScene::update()
{
    InitializationMenuScene *screen = this;
    screen->prepare();
    func_001a78a4(screen, 1);
    screen->finish();
}
