#include "game/display_animation.h"
int func_001672dc(const struct QuestionTable *table, int index)
{
    const struct QuestionRecord *records = table->records;
    return records != 0 ? records[index].enabled : 0;
}
