#include "game/menu_controller.h"



void SelectionMenuScene::update()
{
    SelectionMenuScene *screen = this;
    screen->prepare();
    func_00171a98(screen, 1);
    screen->finish();
}
