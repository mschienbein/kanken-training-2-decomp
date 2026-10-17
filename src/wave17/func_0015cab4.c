struct HwrDictionaryHeader {
    unsigned short field00;
    unsigned short field02;
    unsigned short field04;
    unsigned short field06;
    unsigned short field08;
};

struct HwrDictionaryRecord {
    unsigned char code;
    unsigned char category;
    unsigned char hidden;
    unsigned char reserved03[11];
};

struct HwrDictionaryRecords {
    unsigned char reserved00[0x294];
    struct HwrDictionaryRecord records[];
};

struct HwrDictionaryCounts {
    unsigned char reserved00[0x148a];
    unsigned short counts[];
};

union HwrDictionary {
    struct HwrDictionaryHeader header;
    struct HwrDictionaryRecords record_view;
    struct HwrDictionaryCounts count_view;
};

extern union HwrDictionary *data_0024385c;

void func_0015cab4(void)
{
    data_0024385c->header.field00 = 0;
    data_0024385c->header.field02 = 0;
    data_0024385c->header.field08 = 0;
    data_0024385c->header.field04 = 0;
    data_0024385c->header.field06 = 0;
}
