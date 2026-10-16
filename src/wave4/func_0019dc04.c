struct SharedMemory {
    unsigned reserved00[2];
    unsigned size;
    unsigned char reserved0c[0x74];
    unsigned char memory[];
};

extern struct SharedMemory *data_00242d88;

unsigned char *func_0019dc04(void)
{
    return data_00242d88 ? data_00242d88->memory : 0;
}
