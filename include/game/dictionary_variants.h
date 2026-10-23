#ifndef GAME_DICTIONARY_VARIANTS_H
#define GAME_DICTIONARY_VARIANTS_H

/* Hash-bound incremental author witness; descriptive field/type names remain
 * [INFERENCE]. Shared declarations replace all local copies in collected units. */
struct W20DictionaryRecord {
    int textOffset;
    int alternateOffset;
    unsigned reserved08;
    signed char group, subgroup, variant, reserved0f;
};
struct W20Dictionary {
    unsigned char reserved00[0x10];
    struct W20DictionaryRecord *records;
    int count;
};

#ifdef __cplusplus
extern "C" {
#endif
int func_001dcbf8(const struct W20Dictionary *dictionary, int selected, short *output, int capacity);
#ifdef __cplusplus
}
#endif

#endif
