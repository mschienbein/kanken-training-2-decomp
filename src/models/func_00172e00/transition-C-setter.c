#include "game/transition.h"
struct TransitionCompletion *set_completion(struct TransitionCompletion *state,int completion)
{
    state->completion=completion;
    return state;
}
