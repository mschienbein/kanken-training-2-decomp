struct Entry { int value; int flags; };
struct EntryList { Entry *entries; int count; };
extern EntryList data_0024225c;
extern "C" void *func_0019d798(int heap, int alignment, int size);
extern "C" void func_001033cc(int count)
{
    data_0024225c.entries = static_cast<Entry *>(func_0019d798(2, 4, count * 8));
    data_0024225c.count = count;
    for (int i = 0; i < count; ++i) {
        data_0024225c.entries[i].value = -1;
        data_0024225c.entries[i].flags = 0;
    }
}
