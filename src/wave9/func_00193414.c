struct ResourceIdentity {
    unsigned char reserved00[0x48];
    unsigned short category;
    unsigned short index;
};

/* [INFERENCE] The volatile view retains the evidenced full-width read before truncation. */
unsigned func_00193414(const volatile struct ResourceIdentity *identity)
{
    return identity->category & 0xff;
}
