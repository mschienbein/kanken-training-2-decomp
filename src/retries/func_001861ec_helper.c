#include "game/archive.h"
static __inline struct WordPair *copy_word_pair(struct WordPair *destination,
                                               const struct WordPair *source)
{
    *destination = *source;
    return destination;
}
struct WordPair *func_001861ec(struct WordPair *out, int year, int month, int day,
                              int hour, int minute, int second, int millisecond)
{
    struct WordPair value;
    func_00136cdc(&value, year, month, day, hour, minute, second, millisecond);
    return copy_word_pair(out, &value);
}
