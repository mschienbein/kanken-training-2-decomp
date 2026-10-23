#ifndef GAME_HWR_CANDIDATES_H
#define GAME_HWR_CANDIDATES_H

/* [INFERENCE] Descriptive recognition-list and template identities. */
struct W11StrokeTemplate;
struct W11StrokeCandidate {
    struct W11StrokeTemplate *entry;
    unsigned short score;
    unsigned short first_stroke;
    unsigned short stroke_count;
    unsigned short auxiliary;
};
struct W11CandidateList {
    struct W11StrokeCandidate *items;
    unsigned short capacity;
    unsigned short count;
    unsigned short threshold;
    unsigned short worst_index;
};
struct W11IndexRecord {
    unsigned short position;
    unsigned char reserved02[10];
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct W11CandidateList bss_004dffd0;
extern struct W11CandidateList bss_004e3004;
extern struct W11IndexRecord bss_004e2e8a[];
void func_00157500(int source_index, unsigned first, unsigned cost);
#ifdef __cplusplus
}
#endif

#endif
