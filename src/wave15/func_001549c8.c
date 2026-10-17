struct W15fConfiguration {
    unsigned int reserved00;
    signed char mode;
};

extern struct W15fConfiguration data_00242d90;

int func_001549c8(void)
{
    return data_00242d90.mode;
}
