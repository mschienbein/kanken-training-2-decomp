struct HwrSecondaryRecord {
    void *optional_00;
    void *optional_04;
    void *required_08;
    void *required_0c;
    unsigned short reserved_10;
    unsigned short size;
    unsigned char payload[];
};

struct HwrSecondaryRecord *func_00151a68(struct HwrSecondaryRecord *record)
{
    return (struct HwrSecondaryRecord *)((char *)record + record->size);
}
