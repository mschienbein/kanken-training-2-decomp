#include "game/transition.h"
struct MenuTransitionState *set_completion(struct MenuTransitionState *state,int completion)
{
    state->completion=completion;
    return state;
}
