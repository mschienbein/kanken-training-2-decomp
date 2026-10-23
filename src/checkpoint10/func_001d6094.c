struct PlaybackSlot {
    unsigned char reserved00[0x27];
    unsigned char active;
    unsigned char reserved28[8];
    unsigned first;
    unsigned second;
    unsigned current;
    int selected;
    unsigned resource;
    unsigned reserved44;
    unsigned start;
    unsigned end;
    unsigned char pending;
    unsigned char stopped;
    unsigned char reserved52[0xb94 - 0x52];
    unsigned char state;
    unsigned char reservedb95[0xcf8 - 0xb95];
    int previous;
};
extern void func_001d65e0(struct PlaybackSlot *slot);

void func_001d6094(struct PlaybackSlot *slot)
{
    func_001d65e0(slot);
    slot->current = 0;
    slot->selected = -1;
    slot->resource = 0;
    slot->start = 0;
    slot->end = 0;
    slot->pending = 0;
    slot->stopped = 0;
    slot->active = 0;
    slot->state = 3;
    slot->first = 0;
    slot->previous = -1;
    slot->second = 0;
}
