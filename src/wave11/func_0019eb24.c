struct DisplayQueueCounter {
    unsigned char reserved00[8];
    int count;
};

extern struct DisplayQueueCounter bss_00495c70;

void func_0019eb24(void)
{
    struct DisplayQueueCounter *queue = &bss_00495c70;
    queue->count = 0;
}
