struct ServiceReport;
extern unsigned func_001375b0(unsigned char *out, const struct ServiceReport *report,
                              const char *text);

static __forceinline int result_module(unsigned result)
{
    return (result & 0x3fc00u) >> 10;
}

static __forceinline int result_description(unsigned result)
{
    return result & 0x3ffu;
}

unsigned func_0018fa04(unsigned char *out, const struct ServiceReport *report,
                       const char *text)
{
    unsigned result = func_001375b0(out, report, text);
    if (result_module(result) == 17 &&
        result_description(result) >= 100 && result_description(result) <= 179)
        return 0xc961391au;
    return 0;
}
