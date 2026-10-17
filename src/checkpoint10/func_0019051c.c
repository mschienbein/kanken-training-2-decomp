#include "game/screen.h"
#include <string.h>

/* Layout shared with src/wave5/func_00145840.c. */
struct RequestRecord {
    unsigned kind;
    unsigned flags;
    unsigned short count;
    unsigned short length;
    unsigned short text[0x76c];
    unsigned char flag_ee4;
    unsigned char flag_ee5;
    unsigned char flag_ee6;
    unsigned char auxiliary[0x89];
    int handle;
    unsigned short status;
    unsigned char reservedf76[10];
};
extern void func_001847c8(void *storage, int initial);

void func_0019051c(struct RequestRecord *record)
{
    record->kind = 0;
    record->flags = 0;
    record->count = 0;
    record->length = 0;
    func_0019cecc(record->text, sizeof(record->text));
    record->flag_ee4 = 1;
    record->flag_ee5 = 0;
    record->flag_ee6 = 0;
    memset(record->auxiliary, 0, sizeof(record->auxiliary));
    {
        void *storage = record->reservedf76;
        record->handle = -1;
        record->status = 0;
        func_001847c8(storage, -1);
    }
}
