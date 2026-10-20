#include "game/reset_reference.h"

/* This is the construction callback itself, not an alias or adapter. Its
 * input/result types agree with the actual vector runtime invocation. */
void *func_00156da0(void *storage)
{
    struct ResetReference *reference = storage;
    reference->value = 0;
    return reference;
}
