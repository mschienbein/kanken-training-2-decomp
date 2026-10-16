struct ServiceState {
    unsigned char reserved00;
    unsigned char active;
    unsigned char reserved02[4];
    unsigned char mode;
    unsigned char reserved07[2];
    signed char flag;
    unsigned char reserved0a;
    unsigned char option;
    unsigned char reserved0c[12];
    volatile unsigned state;
};

extern struct ServiceState data_00242a50;

unsigned func_00191de8(void)
{
    return data_00242a50.state;
}
