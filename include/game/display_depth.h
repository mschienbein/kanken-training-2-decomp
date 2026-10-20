#ifndef GAME_DISPLAY_DEPTH_H
#define GAME_DISPLAY_DEPTH_H

/* [INFERENCE] Two eight-entry depth tables are snapshotted before registration. */
struct DisplayDepthArray { float values[8]; };
struct DisplayDepthTable {
    struct DisplayDepthArray upper;
    struct DisplayDepthArray lower;
};

#ifdef __cplusplus
extern "C" {
#endif
extern const struct DisplayDepthTable data_00218000;
void func_0015e2a4(unsigned char enabled);
void func_00181420(int screen, int index, float depth);
#ifdef __cplusplus
}
#endif

#endif
