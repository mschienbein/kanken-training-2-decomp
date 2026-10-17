unsigned int *func_001c511c(unsigned int *command, int enabled)
{
    unsigned int opcode = enabled ? 0x23du : 0x23cu;
    *command++ = 1;
    *command++ = opcode | 0xf0000u;
    return command;
}
