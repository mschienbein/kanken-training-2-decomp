struct AdpcmChannelParameters {
    short coefficients[16];
    short predictor_scale;
    short history1;
    short history2;
};
struct PrefetchChannelReference {
    unsigned short type;
    unsigned short reserved02;
    int offset;
};

const struct AdpcmChannelParameters *func_001547ec(const struct PrefetchChannelReference *reference)
{
    if (reference->type == 0x300)
        return (const struct AdpcmChannelParameters *)((const char *)reference + reference->offset);
    return 0;
}
