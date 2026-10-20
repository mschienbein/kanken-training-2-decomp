struct PendingSelection {
    unsigned char reserved00[0xa0];
    int state;
    unsigned reserveda4;
    int pending;
};

int func_001ada88(struct PendingSelection *selection)
{
    int pending = -1;
    if (selection->state == -1)
        return -1;
    if (selection->pending != -1) {
        pending = selection->pending;
        selection->pending = -1;
    }
    return pending;
}
