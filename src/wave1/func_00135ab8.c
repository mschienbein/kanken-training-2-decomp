typedef struct BufferState {
    void *buffer;
    unsigned int count;
    unsigned int capacity;
} BufferState;

extern BufferState bss_0049cbc0;
extern void func_0019dc20(void *memory);

void func_00135ab8(void)
{
    if (bss_0049cbc0.buffer) {
        func_0019dc20(bss_0049cbc0.buffer);
        bss_0049cbc0.buffer = 0;
        bss_0049cbc0.count = 0;
        bss_0049cbc0.capacity = 0;
    }
}
