#include "game/screen.h"
extern void func_0015e830(struct AlignmentHandle *alignment, unsigned value);

void func_0015e824(struct TrainingWidget *widget, unsigned value)
{
    func_0015e830(widget->bindings[0]->alignment, value);
}
