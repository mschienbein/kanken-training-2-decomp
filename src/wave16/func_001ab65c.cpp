#include "game/menu_controller.h"


void MenuResetScene::prepare()
{
    MenuResetScene *state = this;
    switch (state->depth) {
    case 0:
        func_001ab680(state);
        break;
    case 1:
        func_001abf24(state);
        break;
    }
}
