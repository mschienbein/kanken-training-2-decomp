#include "game/controller.h"
#include "game/screen.h"

struct AnimationBank;
struct MotionRange;
struct MotionTarget;



void ScreenController::prepare()
{
    ScreenController *scene = this;
    switch (scene->selected) {
    case 0:
        func_00161240(scene);
        break;
    case 1:
        func_001608b4(scene);
        break;
    case 2:
        func_00160694(scene);
        break;
    }
}
