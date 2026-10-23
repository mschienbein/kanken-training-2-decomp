#include "game/selection_collections.h"

template<class Handle> static void release_display(Handle *handle)
{
    if (handle && handle->object)
        func_00183208(handle->object);
}

template<class Handle> static void reset_display(Handle *handle)
{
    if (handle)
        handle->object = 0;
}

template<class Handle, int Count>
static void clear_collection(HandleCollection<Handle, Count> *values)
{
    int index = 0;
    do {
        Handle *allocation = values->entries[index];
        if (allocation) {
            ::release_display(values->entries[index]);
            /* Destruction may replace an entry: reset the reloaded handle. */
            ::reset_display(values->entries[index]);
            func_001852f4(allocation);
            values->entries[index] = 0;
        }
    } while (++index < Count);
}

/* Native destructor identities follow the retained registration pointers in
 * matching singleton constructors 001b6400 and 001b695c. Historical spelling
 * remains [INFERENCE], but these are real definitions, not extern-C shadows. */
PairCollection::~PairCollection()
{
    clear_collection<PairHandle, 10>(this);
}

ScalarCollection::~ScalarCollection()
{
    clear_collection<ScalarHandle, 20>(this);
}

extern "C" ChoiceCollection *func_001b7cf8(ChoiceCollection *controller)
{
    clear_collection(controller);
    return controller;
}
