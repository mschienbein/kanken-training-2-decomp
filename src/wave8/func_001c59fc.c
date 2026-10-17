struct PaneStyle;
struct Pane {
    unsigned char reserved00[5];
    unsigned char alignment;
    unsigned char reserved06[0x12];
    int extent;
    unsigned char reserved1c[0x4c];
    struct PaneStyle *style;
};

extern void func_001c5a08(struct PaneStyle *style, unsigned alignment);

void func_001c59fc(struct Pane *pane, unsigned alignment)
{
    pane->alignment = alignment;
    func_001c5a08(pane->style, alignment);
}
