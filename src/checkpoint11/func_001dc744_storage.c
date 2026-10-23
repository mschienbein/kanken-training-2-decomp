#include "game/notification.h"

static struct SavedNotification *result_storage(void)
{
    return &bss_00280af0;
}

static unsigned char *grade_result_flags(int grade)
{
    return result_storage()->payload.storage.fields.result_flags[grade];
}

static unsigned char *result_flag_byte(int grade, int index)
{
    return grade_result_flags(grade) + index / 2;
}

void func_001dc744(int grade, int index, int value)
{
    unsigned char *flags = result_flag_byte(grade, index);
    *flags |= (index & 1) ? 32 : 2;
}

void func_001dc918(int grade, int index, int value)
{
    unsigned char *flags = result_flag_byte(grade, index);
    *flags |= (index & 1) ? 16 : 1;
}
