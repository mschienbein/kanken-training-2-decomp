#ifndef GAME_SELECTOR_LIFECYCLE_H
#define GAME_SELECTOR_LIFECYCLE_H
#include "game/selection.h"
#include "game/resource_runtime.h"
/* [INFERENCE] Qualified source terms, not original C++ names. Factory evidence
 * fixes five mask words, one eight-byte reference and one trailing state word.
 * The reference uses the actual ResourceEvent cleanup interface. */
struct SelectorReference {
    ResourceEvent *event;
    unsigned reserved;
    SelectorReference() : event(0) {}
};
struct SelectorMask {
    int capacity, count;
    unsigned options[5];
    SelectorMask(int entries) : capacity(32), count((entries + capacity - 1) / capacity) {
        for (int i = 0; i < count; ++i)
            options[i] = 0;
    }
};
struct W6Selector : SelectorMask {
    SelectorReference *references[1];
    unsigned active;
    W6Selector() : SelectorMask(138) {
        if (!references[0])
            references[0] = new (data_002232d5) SelectorReference;
        active = 0;
    }
    ~W6Selector();
    SelectorReference *release_reference(int index);
    void reset_reference(int index);
};
extern "C" W6Selector *func_001af370(void);
#endif
