#ifndef GAME_PROPERTY_TEXT_H
#define GAME_PROPERTY_TEXT_H

/* [INFERENCE] Descriptive property roles. AttributeMapping is shared with the
 * accepted attribute-record accessors; the property table is a distinct owner. */
struct AttributeMapping { const char *name; const char *value; };
struct AttributeRecord {
    unsigned char reserved00[0x18];
    const struct AttributeMapping *entries;
    unsigned entry_count;
    unsigned reserved20;
    const unsigned short *children;
    unsigned child_count;
    unsigned reserved2c;
};
struct AttributeCursor { const struct AttributeRecord *record; };
struct AttributeTable {
    const struct AttributeRecord *records;
    unsigned record_count;
};
struct AttributeOwner {
    const void *storage;
    const struct AttributeTable *table;
};
struct PropertyTable { const struct AttributeMapping *entries; unsigned count; };
struct PropertyReference { unsigned reserved00; const struct PropertyTable *table; };
/* The caller supplies a separate 64-byte hidden result, not reference storage. */
struct Text64 { char text[64]; };

#ifdef __cplusplus
extern "C" {
#endif
char *func_0018f7f4(char *destination, const char *source, unsigned size);
int func_0018f860(const char *left, const char *right, unsigned limit);
struct Text64 func_00112fb8(const struct PropertyReference *reference);
#ifdef __cplusplus
}
#endif
#endif
