#include "game/packed_records.h"
#include "game/messages.h"

extern int func_00181aac(int selection);
extern const unsigned short *func_001dcecc(struct PackedRecordSet *list, int value);
extern int func_001dcf0c(struct PackedRecordSet *list, int value);
extern void func_001775e0(struct MessageLayout *layout);
extern const unsigned short data_00227650[];

static void set_animation(struct MessageLayout *layout, int index, float x, float y)
{
    struct MessageEntry *entry = layout->entries[index];
    if (entry != 0 && entry->object != 0) {
        entry->animation_x = x;
        entry->animation_y = y;
    }
}

void func_001773cc(struct MessageLayout *layout, struct PackedRecordSet *list, int character)
{
    if (character != 0) {
        const unsigned short *text;
        int flag;
        float x, y;
        func_00181aac(1);
        func_00170dc0(layout, 1, 1);
        text = func_001dcecc(list, character);
        func_0017a43c(layout, L"\n", text, 0);
        func_001775e0(layout);
        flag = func_001dcf0c(list, character);
        y = -132.0f;
        x = (float)(flag + 3) * -24.0f - 128.0f;
        set_animation(layout, 2, x, y);
    } else {
        func_00170dc0(layout, 1, 0);
        func_0017a43c(layout, data_00227650, 0, 0);
    }
}
