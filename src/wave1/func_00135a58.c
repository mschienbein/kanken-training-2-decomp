typedef struct BufferState {
    void *buffer;
    unsigned int count;
    unsigned int capacity;
} BufferState;

extern BufferState bss_0049da58;
extern void func_0019dc20(void *memory);

void func_00135a58(void)
{
    if (bss_0049da58.buffer) {
        func_0019dc20(bss_0049da58.buffer);
        bss_0049da58.buffer = 0;
        bss_0049da58.count = 0;
        bss_0049da58.capacity = 0;
    }
}
