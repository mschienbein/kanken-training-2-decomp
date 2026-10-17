struct ResetCounters {
    int first[3];
    int second[3];
};

extern struct ResetCounters bss_002d4250;

void func_0010600c(void)
{
    bss_002d4250.first[0] = 0;
    bss_002d4250.second[0] = 0;
    bss_002d4250.first[1] = 0;
    bss_002d4250.second[1] = 0;
    bss_002d4250.first[2] = 0;
    bss_002d4250.second[2] = 0;
}
