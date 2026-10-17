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

void func_001d4aac(struct PaneGroup *group)
{
    group->active = 1;
    group->pending = 0;
    group->flags |= 1;
}
