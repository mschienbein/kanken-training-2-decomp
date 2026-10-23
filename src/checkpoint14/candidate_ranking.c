#include "game/hwr_candidates.h"

void func_00157500(int source_index, unsigned int first, unsigned int cost)
{
    unsigned int count = bss_004dffd0.count;
    struct W11StrokeCandidate *items = bss_004dffd0.items;
    unsigned short score = items[source_index].score;
    unsigned short position = 0;
    for (; position < count; ++position) {
        if (items[position].score > score || items[position].stroke_count > cost)
            goto insert;
        if (items[position].first_stroke == first)
            return;
    }
store:
    if (position < bss_004dffd0.capacity) {
        items[position].score = score;
        items[position].first_stroke = first;
        items[position].stroke_count = cost;
        items[position].auxiliary = 0xff;
        items[position].entry = (struct W11StrokeTemplate *)-1;
        if (bss_004dffd0.count < 32)
            ++bss_004dffd0.count;
    }
    return;

insert:
    {
        unsigned short duplicate;
        unsigned short move;
        struct W11IndexRecord *record;
        for (duplicate = position; count > duplicate; ++duplicate) {
            if (items[duplicate].first_stroke == first)
                break;
        }
        if (count != duplicate) {
            for (; count > duplicate + 1; ++duplicate) {
                items[duplicate] = items[duplicate + 1];
                count = bss_004dffd0.count;
            }
            bss_004dffd0.count = bss_004dffd0.count - 1;
        }
        for (move = 31; move > position; --move)
            items[move] = items[move - 1];
        count = bss_004e3004.count;
        if (count != 0) {
            for (record = bss_004e2e8a; count != 0; --count) {
                if (record->position >= position)
                    ++record->position;
                ++record;
            }
        }
    }
    goto store;
}
