#include "game/runtime.h"
struct RegionRecord {
    unsigned start;
    unsigned size;
    unsigned attributes;
};
struct RegionTracker {
    unsigned enabled;
    unsigned ready;
    unsigned char *begin[2];
    unsigned size[2];
    struct RegionRecord records[2][65];
};


void func_001102c8(struct RegionTracker *tracker)
{
    unsigned index;
    unsigned char *end;
    tracker->begin[0] = func_0019cff8(0x20000);
    tracker->begin[1] = func_0019cff8(0x30000);
    tracker->size[0] = (unsigned char *)func_00190ae0(0x20000) - tracker->begin[0];
    end = func_00190ae0(0x30000);
    tracker->ready = 1;
    tracker->size[1] = end - tracker->begin[1];
    tracker->enabled = 1;
    for (index = 0; index < 65; ++index) {
        tracker->records[1][index].start = 0;
        tracker->records[0][index].start = 0;
        tracker->records[1][index].size = 0;
        tracker->records[0][index].size = 0;
        tracker->records[1][index].attributes = 0;
        tracker->records[0][index].attributes = 0;
    }
}
