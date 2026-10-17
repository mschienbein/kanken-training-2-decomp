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
/* [INFERENCE] The same workspace base is used with indexed tree and stroke
 * geometry views. The union expresses overlapping representation, not two
 * allocations or a recovered phase/lifetime rule. Both following vectors are
 * variable-length; no bound is invented for the tree or offset table. */
union HwrWorkspace {
    struct HwrTree tree;
    struct HwrStrokeGeometry geometry;
};
struct HwrCachedBounds { unsigned short max_y, max_x, min_y, min_x; };
#ifdef __cplusplus
extern "C" {
#endif
extern union HwrWorkspace *data_0024385c;
extern union HwrWorkspace bss_004e07f0;
extern unsigned short *data_00243888;
extern struct HwrCachedBounds bss_004e1e04[];
extern struct RecognitionState bss_004e071c;
#ifdef __cplusplus
}
#endif
#endif
