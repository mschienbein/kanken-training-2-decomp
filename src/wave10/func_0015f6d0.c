struct TrainingTextData {
    unsigned char reserved00[0x18];
    const unsigned short *const *rows;
    const signed char *types;
};

int func_0015f6d0(const struct TrainingTextData *data, int row, int column)
{
    const unsigned short *text = data->rows[row];
    int character = 0;
    if (text != 0)
        character = text[column];
    return character;
}
