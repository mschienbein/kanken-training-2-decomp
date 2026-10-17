struct ExtendedWorkspace {
    unsigned reserved00;
    unsigned first_duration;
    unsigned reserved08;
    unsigned second_duration;
    unsigned reserved10[6];
    void *memory;
    unsigned reserved2c;
    unsigned first_count;
    unsigned second_count;
    unsigned third_count;
    unsigned char *buffer;
    unsigned char *second_buffer;
    unsigned reserved44[2];
    unsigned char *first;
    unsigned char *second_first;
    unsigned reserved54[2];
    unsigned *second;
    unsigned *third;
    unsigned *second_second;
    unsigned *second_third;
    unsigned reserved6c[4];
    unsigned *fourth;
    unsigned *second_fourth;
    unsigned char reserved84[0x6c];
    unsigned saved_first_duration;
    unsigned saved_second_duration;
    unsigned saved_first_count;
    unsigned saved_second_count;
    unsigned saved_third_count;
    unsigned char initialized;
};

void func_0017e8bc(struct ExtendedWorkspace *workspace)
{
    if (workspace->initialized) {
        workspace->initialized = 0;
        workspace->buffer = 0;
        workspace->first = 0;
        workspace->second = 0;
        workspace->third = 0;
        workspace->fourth = 0;
        workspace->second_buffer = 0;
        workspace->second_first = 0;
        workspace->second_second = 0;
        workspace->second_third = 0;
        workspace->second_fourth = 0;
    }
}
