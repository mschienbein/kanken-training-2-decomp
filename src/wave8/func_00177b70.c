struct AnimationModeRecord {
    unsigned char reserved00[0x28];
    int mode;
    unsigned char reserved2c[0x28];
};

int func_00177b70(const struct AnimationModeRecord *records, int index)
{
    return records[index].mode;
}
