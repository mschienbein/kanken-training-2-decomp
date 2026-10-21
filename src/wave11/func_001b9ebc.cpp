#include "game/training_screen.h"
void TrainingScreenA::update()
{
    TrainingScreenA *screen = this;
    screen->prepare();
    func_001b0600(screen->small_controls);
    if (screen->small_controls != 0 && screen->animation != 0)
        func_0016d554(screen->small_controls, func_0017a210(screen->animation));
    screen->finish();
}
