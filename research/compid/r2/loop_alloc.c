struct Entry { int value; int flags; };
struct EntryList { struct Entry *entries; int count; };
extern struct EntryList data_0024225c;
extern void *func_0019d798(int heap, int alignment, int size);

void func_001033cc(int count)
{
    int i;
    struct Entry *entries = func_0019d798(2, 4, count * sizeof(struct Entry));
    data_0024225c.entries = entries;
    data_0024225c.count = count;
    for (i = 0; i < count; ++i) {
        entries[i].value = -1;
        entries[i].flags = 0;
    }
}
