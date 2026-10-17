struct W17Command {
    unsigned int operation;
    unsigned int flags;
    unsigned int reserved08;
    int first;
    int second;
    unsigned int reserved14[3];
};

struct W17CommandQueue {
    unsigned int reserved00[0x1024 / 4];
    int write_index;
    int read_index;
    struct W17Command commands[];
};

extern struct W17CommandQueue bss_004990d0;

void func_001865ec(int first, int second)
{
    struct W17Command *command = &bss_004990d0.commands[bss_004990d0.write_index];
    bss_004990d0.write_index = (bss_004990d0.write_index + 1) % 256;
    command->operation = 2;
    command->flags = 0;
    command->first = first;
    command->second = second;
}
