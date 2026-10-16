struct Entry { int id; void *resource; };
struct EntryList { struct Entry *entries; int count; };
extern struct EntryList data_0024225c;
extern void *func_0019d798(int heap, int alignment, int size);
void func_001033cc(int count)
{
    int i;
    data_0024225c.entries = func_0019d798(2, 4, count * 8);
    data_0024225c.count = count;
    for (i = 0; i < count; ++i) {
        data_0024225c.entries[i].id = -1;
        data_0024225c.entries[i].resource = 0;
    }
}
