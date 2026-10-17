#include "game/runtime.h"
#include "game/text.h"
#include <string.h>

struct L041ReadingEntry {
    int present;
    unsigned katakana_offset;
    unsigned hiragana_offset;
    unsigned reserved0c;
    unsigned short identifier;
    unsigned short reserved12;
};

struct L041ReadingDictionary {
    unsigned reserved00[3];
    const struct L041ReadingEntry *entries;
};

extern int func_0016e5cc(const unsigned short *left, const unsigned short *right, int count);
extern const unsigned short *func_0015dd5c(const unsigned short *text, unsigned short character);
extern void func_0019e08c(void *allocation);

static __forceinline int append_identifier(unsigned short *results, int capacity, unsigned short identifier)
{
    int i, j, count;
    for (i = 0; i < capacity; ++i) {
        if (results[i] == 0) {
            for (j = i; j < capacity - 1; ++j)
                results[j] = results[j + 1];
            results[capacity - 1] = 0;
        }
    }
    for (i = 0; i < capacity; ++i) {
        if (results[i] == identifier)
            break;
        if (results[i] == 0) {
            results[i] = identifier;
            break;
        }
    }
    count = 0;
    for (i = 0; i < capacity; ++i) {
        if (results[i] == 0)
            break;
        ++count;
    }
    return count;
}

static __forceinline unsigned to_hiragana(unsigned short character)
{
    if ((unsigned)(character - 0x30a1) <= 0x55)
        return character - 0x60;
    return character;
}

static __forceinline unsigned to_katakana(unsigned short character)
{
    if ((unsigned)(character - 0x3041) <= 0x55)
        return character + 0x60;
    return character;
}

static __inline void normalize_readings(const unsigned short *text, unsigned short *hiragana, unsigned short *katakana, int length)
{
    int remaining = length;
    do {
        *hiragana = to_hiragana(*text);
        *katakana = to_katakana(*text);
        ++hiragana;
        ++text;
        ++katakana;
    } while (--remaining != 0);
}

int func_001dcfc0(const struct L041ReadingDictionary *dictionary,
                  const unsigned short *text, unsigned short *results, int capacity)
{
    int count;
    int length;
    unsigned short *hiragana, *katakana;
    const struct L041ReadingEntry *entry;
    if (results != 0)
        memset(results, 0, capacity * sizeof(*results));
    if (dictionary->entries == 0)
        return 0;
    length = func_0017adc4(text);
    if (length == 0)
        return 0;
    count = 0;
    hiragana = func_00193110(length * sizeof(*hiragana), data_002232d5);
    katakana = func_00193110(length * sizeof(*katakana), data_002232d5);
    /* ORIGINAL BUG: both allocations are used without checking for failure. */
    memset(hiragana, 0, length * sizeof(*hiragana));
    memset(katakana, 0, length * sizeof(*katakana));
    normalize_readings(text, hiragana, katakana, length);
    for (entry = dictionary->entries; entry->present != 0; ++entry) {
        const unsigned short *reading;
        if (entry->hiragana_offset != 0) {
            reading = (const unsigned short *)((const char *)dictionary->entries + entry->hiragana_offset);
            while (reading != 0 && *reading != 0) {
                if (func_0016e5cc(reading, hiragana, length) == 0) {
                    if (results == 0)
                        ++count;
                    else
                        count = append_identifier(results, capacity, entry->identifier);
                }
                reading = func_0015dd5c(reading, 0x20);
                if (reading == 0)
                    break;
                ++reading;
            }
        }
        if (entry->katakana_offset != 0) {
            reading = (const unsigned short *)((const char *)dictionary->entries + entry->katakana_offset);
            while (reading != 0 && *reading != 0) {
                if (func_0016e5cc(reading, katakana, length) == 0) {
                    if (results == 0)
                        ++count;
                    else
                        count = append_identifier(results, capacity, entry->identifier);
                }
                reading = func_0015dd5c(reading, 0x20);
                if (reading == 0)
                    break;
                ++reading;
            }
        }
    }
    func_0019e08c(hiragana);
    func_0019e08c(katakana);
    return count;
}
