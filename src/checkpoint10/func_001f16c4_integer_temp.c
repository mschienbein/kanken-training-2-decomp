#include <stdint.h>

struct EncodedLayout {
    unsigned char reserved00[0x2c];
    unsigned format;
    unsigned reserved30;
    unsigned char *base;
};
unsigned char *func_001f16c4(const struct EncodedLayout *layout)
{
    unsigned format = layout->format;
    unsigned offset = ((format >> 2) & 12u) + (((format & 3u) * 32u) + (((format >> 2) & 3u) * 20u));
    return (unsigned char *)(offset + (uintptr_t)layout->base);
}
