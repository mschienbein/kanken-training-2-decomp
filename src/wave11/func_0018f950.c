#include "game/archive.h"

struct ServiceReport {
    unsigned reserved00[8];
    int state;
    struct WordPair result;
};
extern void func_00194a34(struct WordPair *out, unsigned result);

static __forceinline int result_failure(unsigned result)
{
    return (int)(result & 0x80000000u);
}

void func_0018f950(struct WordPair *out, const struct ServiceReport *report)
{
    switch (report->state) {
    case 1:
        func_00194a34(out, 0);
        return;
    case 0:
        func_00194a34(out, 0xc9213801u);
        return;
    case 10:
        func_00194a34(out, 0xc9213802u);
        return;
    case 11:
        func_00194a34(out, 0xc9213803u);
        return;
    case 12:
        func_00194a34(out, 0xc9213804u);
        return;
    case -2:
        {
            unsigned result = report->result.first;
            if (result_failure(result) >= 0)
                func_00194a34(out, 0xf9613919u);
            else
                func_00194a34(out, result);
            return;
        }
    case -3:
        func_00136448(out, report->result.first, report->result.second);
        return;
    default:
        func_00194a34(out, 0xc9613918u);
        return;
    }
}
