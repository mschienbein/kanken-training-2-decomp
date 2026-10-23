#include "game/selector_lifecycle.h"

inline void W6Selector::reset_reference(int index)
{
    SelectorReference *reference = references[index];
    if (reference)
        reference->event = 0;
}
inline SelectorReference *W6Selector::release_reference(int index)
{
    SelectorReference *reference = references[index];
    ResourceEvent *event = reference->event;
    if (event)
        func_00183208(event);
    reset_reference(index);
    return reference;
}
W6Selector::~W6Selector()
{
    for (int index = 0; index < 1; ++index) {
        SelectorReference *reference = references[index];
        if (reference) {
            reference = release_reference(index);
            func_001852f4(reference);
            references[index] = 0;
        }
    }
}
extern "C" W6Selector *func_001af370(void)
{
    /* [INFERENCE] Destructor registration and guard release were inlined by
     * armlink; the retained registered destructor pointer fixes its target. */
    static W6Selector instance;
    return &instance;
}
