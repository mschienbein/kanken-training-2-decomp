#include "game/reset_reference.h"

/* The vector-construction runtime consumes the callback's initialized object. */
struct ResetReference *func_00156da0(struct ResetReference *reference)
{
    reference->value = 0;
    return reference;
}
