#include <string.h>

__packed struct PackedDefaults {
    unsigned char first[5];
    unsigned short cleared;
    unsigned char second[4];
    unsigned char suffix[2];
};

struct PackedDefaults *func_001d0110(struct PackedDefaults *settings)
{
    settings->first[0] = 0;
    settings->first[1] = 0;
    settings->first[2] = 0;
    settings->first[3] = 0;
    settings->first[4] = 127;
    settings->second[0] = 64;
    settings->second[1] = 0;
    settings->second[2] = 0;
    settings->second[3] = 0;
    memset(settings->suffix, -1, 2);
    settings->cleared = 0;
    return settings;
}
