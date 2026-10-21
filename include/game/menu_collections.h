#ifndef GAME_MENU_COLLECTIONS_H
#define GAME_MENU_COLLECTIONS_H
#include <stdint.h>
#include "game/screen.h"

/* Distinct allocations, established by 0015eae0 (24 four-byte cells),
 * 001671d4 (29 four-byte cells), 001af370 (one eight-byte reference),
 * and 001aff94 (ten eight-byte label handles), not by equal prefixes. */
struct MaskHeader { int bits_per_word, word_count; };
struct SelectionCell { struct DisplayObject *object; };
struct SelectionList24 {
    struct MaskHeader mask;
    unsigned selection_word;
    struct SelectionCell *slots[24];
    const struct NameTable *const *labels;
};
struct SelectionList29 {
    struct MaskHeader mask;
    unsigned selection_word;
    struct SelectionCell *slots[29];
    const struct NameTable *const *labels;
};
struct SelectorReference {
    struct DisplayObject *object;
    unsigned reserved04;
};
struct W6Selector {
    struct MaskHeader mask;
    unsigned selection_words[5];
    struct SelectorReference *reference;
    const struct NameTable **tables;
};
struct SmallLabelHandle {
    struct DisplayObject *object;
    int16_t label_index;
    uint8_t enabled, reserved07;
};

#endif
