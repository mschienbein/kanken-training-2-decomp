#ifndef GAME_SELECTION_COLLECTIONS_H
#define GAME_SELECTION_COLLECTIONS_H
#include "game/runtime.h"
#include "game/resource_runtime.h"

/* [INFERENCE] Allocation-backed owner names. Null initialization and the
 * actual cleanup endpoint establish ResourceEvent pointers here; these small
 * allocations are not aliases of the larger DisplayHandle or WidgetSlot. */
struct SelectionCell {
    ResourceEvent *object;
    SelectionCell() : object(0) {}
};
struct SelectionSet2 {
    int bits_per_word, word_count;
    unsigned masks[1];
    SelectionCell *cells[2];
};
struct MaskDimensions {
    int bits_per_word, word_count;
    unsigned mask[1];
    MaskDimensions(int width) {
        bits_per_word = width;
        word_count = (width + 31) / 32;
        for (int i = 0; i < word_count; ++i)
            mask[i] = 0;
    }
};
struct PairHandle {
    ResourceEvent *object;
    unsigned reserved;
    PairHandle() : object(0) {}
};
struct ScalarHandle {
    ResourceEvent *object;
    ScalarHandle() : object(0) {}
};
struct ChoiceDisplayHandle {
    ResourceEvent *object;
    unsigned short selected;
    bool active;
    ChoiceDisplayHandle() : object(0), selected(0xffff), active(false) {}
};
template<class Handle, int Count>
struct HandleCollection : MaskDimensions {
    Handle *entries[Count];
    HandleCollection() : MaskDimensions(32) {
        for (int i = 0; i < Count; ++i)
            if (!entries[i])
                entries[i] = new (data_002232d5) Handle;
    }
};
struct PairCollection : HandleCollection<PairHandle, 10> { ~PairCollection(); };
struct ScalarCollection : HandleCollection<ScalarHandle, 20> { ~ScalarCollection(); };
typedef HandleCollection<ChoiceDisplayHandle, 55> ChoiceCollection;
extern "C" {
SelectionSet2 *func_001aced0(SelectionSet2 *state);
PairCollection *func_001b6400(void);
ScalarCollection *func_001b695c(void);
ChoiceCollection *func_001b7cf8(ChoiceCollection *collection);
}
#endif
