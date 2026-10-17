#include "game/screen.h"
extern void func_0015e60c(struct AlignmentHandle *alignment, float *width, float *height);

void func_0015e5f0(struct TrainingWidget *widget, unsigned index,
                   float *width, float *height)
{
    func_0015e60c(widget->bindings[index]->alignment, width, height);
}
