#ifndef GAME_HWR_RUNTIME_H
#define GAME_HWR_RUNTIME_H
#include <stdbool.h>
#include <stdint.h>

struct RecognitionPattern;
struct RecognitionCandidate {
    unsigned short character;
    unsigned char reserved02, alternate_count;
    unsigned reserved04;
    const struct RecognitionPattern *pattern;
    const struct RecognitionCandidate *next;
    unsigned char reserved10[11];
    unsigned char direction_count;
#ifndef __cplusplus
    unsigned char directions[];
#endif
};
struct RecognitionEntry {
    const struct RecognitionCandidate *candidate;
    unsigned short score, first, second, flags;
};
struct RecognitionList {
    struct RecognitionEntry *entries;
    unsigned short capacity, count, worst_score, worst_index;
};
/* [INFERENCE] One candidate workspace rooted at bss_004de5cc and published by
 * func_0014db18; entry rows precede the per-stroke list descriptors. */
struct HwrCandidateStorage {
    struct RecognitionEntry entries[32][16];
    struct RecognitionList lists[32];
};
struct RecognitionState {
    unsigned char point_data[0x40];
    unsigned char *points;
    unsigned short point_count;
    unsigned char reserved46[4];
    unsigned short first_stroke, stroke_count;
    unsigned short starts[31], ends[31];
    unsigned char reservedca[6];
    struct RecognitionList *candidates;
};
struct HwrTreeNode {
    unsigned short field00, previous, next, weight;
    signed char marker;
    unsigned char scaled_weight;
    bool active;
    unsigned char reserved0b[3];
};
struct HwrTree {
    unsigned char reserved00[0x28c];
#ifndef __cplusplus
    struct HwrTreeNode nodes[];
#endif
};
struct HwrStrokeGeometry {
    unsigned char reserved00[0x154a];
    unsigned char min_x[32], min_y[32], max_x[32], max_y[32];
    unsigned char reserved15ca[8];
#ifndef __cplusplus
    unsigned short offsets[];
#endif
};
struct HwrStrokeLengths {
    unsigned char reserved00[0x148a];
#ifndef __cplusplus
    unsigned short lengths[];
#endif
};
/* [INFERENCE] The same workspace base is used with indexed tree and stroke
 * geometry views. The union expresses overlapping representation, not two
 * allocations or a recovered phase/lifetime rule. Both following vectors are
 * variable-length; no bound is invented for the tree or offset table. */
struct HwrDictionaryHeader {
    unsigned short field00, field02, field04, field06, field08;
};
union HwrWorkspace {
    struct HwrTree tree;
    struct HwrStrokeGeometry geometry;
    struct HwrDictionaryHeader header;
    struct HwrStrokeLengths lengths;
};
struct HwrCachedBounds { unsigned short max_y, max_x, min_y, min_x; };

/* The reset/configuration/state-machine consumers use the same halfwords.
 * Current/previous are phase-dependent words, not separate shadow owners. */
struct HwrConfiguration {
    unsigned short state, current, previous;
    short width, height;
    unsigned short reserved0a;
    int area;
    unsigned short flags, mode_b, mode_a, limit_a, limit_b;
};
struct HwrContext {
    unsigned short reserved00, strokes, points;
    unsigned char reserved06[0x16];
    unsigned short encoded_size;
    unsigned char reserved1e[0xa];
    unsigned short *group_count;
};
struct HwrCandidate {
    unsigned reserved00;
    unsigned short score, identifier;
    unsigned reserved08;
};
#ifdef __cplusplus
extern "C" {
#endif
extern union HwrWorkspace *data_0024385c;
extern struct HwrCandidateStorage *data_00243830;
extern struct HwrCandidateStorage bss_004de5cc;
extern unsigned char *data_00243864;
extern union HwrWorkspace bss_004e07f0;
extern unsigned short *data_00243888;
extern struct HwrCachedBounds bss_004e1e04[];
extern struct RecognitionState bss_004e071c;
extern struct HwrConfiguration bss_004bfce0;
extern struct HwrContext data_00243834;
extern int func_00159e94(struct HwrCandidate *output, int capacity);
extern void func_0015b950(unsigned short group_count, int option);
extern void func_0017d8a8(unsigned short index);
extern void func_0015c5a4(struct RecognitionList *candidates);
extern void func_0015c734(unsigned int unused_mode, unsigned int mode);
extern void func_0017b4ac(struct RecognitionList *candidates, short first, short last);
#ifdef __cplusplus
}
#endif
#endif
