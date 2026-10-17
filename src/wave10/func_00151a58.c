struct HwrPointerPair {
    void *first;
    void *second;
};

struct HwrPrimaryRecord {
    unsigned short kind;
    unsigned char reserved_02;
    unsigned char has_pair;
    unsigned reserved_04;
    unsigned short word_count;
    unsigned short reserved_0a;
    void *optional_0c;
    struct HwrPointerPair pair;
    unsigned words[];
};

struct HwrPrimaryRecord *func_00151a58(struct HwrPrimaryRecord *record)
{
    return (struct HwrPrimaryRecord *)(record->words + record->word_count);
}
