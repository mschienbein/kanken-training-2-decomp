#include "game/screen.h"
extern void func_0015e78c(struct AlignmentHandle *alignment, unsigned value);

void func_0015e780(struct TrainingWidget *widget, unsigned value)
{
    func_0015e78c(widget->bindings[0]->alignment, value);
}
