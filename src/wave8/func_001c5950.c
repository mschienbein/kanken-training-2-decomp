struct PaneStyle;
struct Pane {
    unsigned char reserved00[5];
    unsigned char alignment;
    unsigned char reserved06[0x12];
    int extent;
    unsigned char reserved1c[0x4c];
    struct PaneStyle *style;
};

extern void func_001c5958(struct PaneStyle *style, unsigned mode);

void func_001c5950(struct Pane *pane, unsigned mode)
{
    func_001c5958(pane->style, mode);
}
