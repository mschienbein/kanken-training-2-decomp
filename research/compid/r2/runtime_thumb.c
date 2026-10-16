extern int *func_001a0098(void);
int func_00181e94(void) { return *func_001a0098(); }
void func_00181e9e(int value) { *func_001a0098() = value; }

unsigned int func_0019d5b0(const unsigned short *text)
{
    unsigned int length = 0;
    while (*text) {
        ++text;
        ++length;
    }
    return length;
}

int func_00191c80(int *guard)
{
    if (*guard)
        return 0;
    *guard = 1;
    return 1;
}
