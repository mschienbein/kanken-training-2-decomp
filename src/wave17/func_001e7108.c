struct PlaybackEntry {
    unsigned char active;
    signed char index;
    unsigned char reserved02[6];
    unsigned short first;
    unsigned short second;
    int identifier;
    unsigned field10;
    unsigned field14;
    unsigned field18;
};

struct PlaybackEntry *func_001e7108(struct PlaybackEntry *entry)
{
    entry->active = 0;
    entry->index = -1;
    entry->first = 0;
    entry->second = 0;
    entry->field10 = 0;
    entry->identifier = -1;
    entry->field14 = 0;
    entry->field18 = 0;
    return entry;
}
