struct FatalErrorInfo {
    unsigned char type;
    unsigned char reserved;
    unsigned short version;
    unsigned result;
    unsigned address;
};

extern struct FatalErrorInfo bss_004b1200;
extern void func_00186294(struct FatalErrorInfo *info);

static __forceinline struct FatalErrorInfo *prepare_error(struct FatalErrorInfo *info,
                                                         unsigned result,
                                                         unsigned address)
{
    info->reserved = 0;
    info->version = 0xf2ad;
    info->type = 0;
    info->address = address;
    info->result = result;
    return info;
}

void func_0019d6c4(unsigned result, unsigned address)
{
    func_00186294(prepare_error(&bss_004b1200, result, address));
}
