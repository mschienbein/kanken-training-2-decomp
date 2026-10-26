#include "game/service_state.h"

static void copyText(unsigned short *destination, const unsigned short *text,
                     unsigned remaining)
{
    while (*text != 0 && remaining != 0) {
        --remaining;
        *destination++ = *text++;
    }
    *destination = 0;
}

void func_00145840(const unsigned short *text)
{
    unsigned char output;
    struct RequestRecord record;
    func_0019051c(&record);
    record.kind = 1;
    copyText(record.text, text, 0x3b6);
    func_001904ec();
    func_001904e0();
    func_00190450(&output, &record);
}
