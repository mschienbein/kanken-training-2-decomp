struct ServiceReport;
extern unsigned func_0013eab4(unsigned char *out, const struct ServiceReport *report,
                              const char *text, float fade_seconds);

unsigned func_001375b0(unsigned char *out, const struct ServiceReport *report,
                       const char *text)
{
    return func_0013eab4(out, report, text, 0.2f);
}
