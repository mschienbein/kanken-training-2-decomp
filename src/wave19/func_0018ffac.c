union RandomSeed {
    unsigned short unsigned_value;
    short signed_value;
};
extern union RandomSeed data_00242240;

int func_0018ffac(int limit)
{
    unsigned short value;
    data_00242240.unsigned_value = data_00242240.unsigned_value * 549 - 1;
    value = data_00242240.unsigned_value;
    if (limit == 0)
        return data_00242240.signed_value;
    if (limit > 0)
        return value * limit / 65536;
    return data_00242240.signed_value * limit / 32768;
}
