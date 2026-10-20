#ifndef GAME_PACKED_RECORDS_H
#define GAME_PACKED_RECORDS_H

struct PackedRecord {
    int text_offset;
    unsigned reserved04[2];
    int value;
    unsigned short identifier;
    unsigned char reserved12;
    signed char flag;
};
struct PackedRecordSet {
    unsigned char reserved00[0x0c];
    struct PackedRecord *records;
    int count;
};
#ifdef __cplusplus
extern "C" {
#endif
extern int func_00169878(struct PackedRecordSet *list, int value);
extern int func_00169874(struct PackedRecordSet *list, int value);
#ifdef __cplusplus
}
#endif

#endif
