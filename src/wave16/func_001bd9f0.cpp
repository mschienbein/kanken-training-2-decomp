#include "game/menu_controller.h"


void ExitSelectionScene::prepare()
{
    ExitSelectionScene *state = this;
    switch (state->depth) {
    case 0:
        func_001bda14(state);
        break;
    case 1:
        func_001bdb70(state);
        break;
    }
}
