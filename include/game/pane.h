#ifndef GAME_PANE_H
#define GAME_PANE_H
struct PaneStyle;
struct Pane {
    unsigned char reserved00[5];
    unsigned char alignment;
    unsigned char reserved06[0x12];
    int extent;
    unsigned char reserved1c[0x4c];
    struct PaneStyle *style;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_001c5930(struct PaneStyle *style, int extent);
extern void func_001c59fc(struct Pane *pane, unsigned alignment);
#ifdef __cplusplus
}
#endif
#endif
