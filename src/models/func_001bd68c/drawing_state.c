#include "game/drawing_screen.h"

/* [INFERENCE] Candidate-only processing-state accessor. No historical callee
 * identity or original address is claimed. Its C linkage agrees with the
 * explicit declaration used by the C++ callers. */
int r41262_drawing_state(const struct W10DrawingInput *input)
{
    return input->state;
}
