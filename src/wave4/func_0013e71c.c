struct SharedMemory {
    unsigned reserved00[2];
    unsigned size;
    unsigned char reserved0c[0x74];
    unsigned char memory[];
};

extern struct SharedMemory *data_00242d88;

unsigned func_0013e71c(void)
{
    return data_00242d88 ? data_00242d88->size : 0;
}
