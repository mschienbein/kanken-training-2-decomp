struct PaneStyle;
struct Pane {
    unsigned char reserved00[5];
    unsigned char alignment;
    unsigned char reserved06[0x12];
    int extent;
    unsigned char reserved1c[0x4c];
    struct PaneStyle *style;
};

extern void func_001c5978(struct PaneStyle *style, unsigned direction);

void func_001c5970(struct Pane *pane, unsigned direction)
{
    func_001c5978(pane->style, direction);
}
