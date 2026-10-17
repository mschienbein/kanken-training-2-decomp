struct Pane;
struct PaneGroup {
    struct Pane *panes[2];
    int count;
    unsigned char reserved0c[9];
    unsigned char active;
    unsigned char reserved16;
    unsigned char pending;
    unsigned char complete;
    unsigned char reserved19[7];
    unsigned short flags;
    unsigned char reserved22[0x2e];
    unsigned char mode;
};

extern void func_001c59fc(struct Pane *pane, unsigned alignment);

void func_001d4a48(struct PaneGroup *group, unsigned alignment)
{
    int index;
    for (index = 0; index < group->count; ++index) {
        struct Pane *pane = group->panes[index];
        if (pane != 0) {
            unsigned mode = 0;
            switch (alignment) {
            case 0: break;
            case 1: mode = 1; break;
            case 2: mode = 2; break;
            default: break;
            }
            func_001c59fc(pane, mode);
        }
    }
}
