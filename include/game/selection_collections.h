#ifndef GAME_SELECTION_COLLECTIONS_H
#define GAME_SELECTION_COLLECTIONS_H
#include "game/runtime.h"
#include "game/menu_collections.h"
#include "game/pair_collection_data.h"

#ifdef __cplusplus
/* These native owner/template identities are preserved from the c011 model.
 * Their singleton factories and native destructors are not re-entered here. */
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
    DisplayObject *object;
    unsigned reserved;
    PairHandle() : object(0) {}
};
struct ScalarHandle {
    DisplayObject *object;
    ScalarHandle() : object(0) {}
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
struct PairCollection {
    PairCollectionData data;
    PairCollection() {
        data.dimensions.bits_per_word = 32;
        data.dimensions.word_count = 1;
        data.mask = 0;
        for (int i = 0; i < 10; ++i)
            if (!data.entries[i])
                data.entries[i] = new (data_002232d5) PairHandle;
    }
    ~PairCollection();
};
struct ScalarCollection : HandleCollection<ScalarHandle, 20> {
    ~ScalarCollection();
};
extern "C" {
PairCollection *func_001b6400(void);
ScalarCollection *func_001b695c(void);
}
#endif
#endif
