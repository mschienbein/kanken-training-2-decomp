struct TrainingTextData {
    unsigned char reserved00[0x18];
    const unsigned short *const *rows;
    const signed char *types;
};

int func_0015f834(const struct TrainingTextData *data, int index)
{
    return data->types[index];
}
