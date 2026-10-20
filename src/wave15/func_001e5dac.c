struct SelectionDataSource {
    unsigned char kind;
    unsigned char reserved01[0x17];
    void *data;
};

extern void func_001e5ec0(struct SelectionDataSource *source, int index);

void func_001e5dac(struct SelectionDataSource *source, int index)
{
    if (source->data && source->kind == 5)
        func_001e5ec0(source, index);
}
