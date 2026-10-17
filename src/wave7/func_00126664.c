struct BoundedText {
    unsigned reserved00;
    char text[];
};

struct TextObject {
    unsigned char reserved00[0x10];
    const struct BoundedText *text;
    unsigned length;
};

struct TextReference {
    const struct TextObject *object;
};

extern char *func_0018f7f4(char *destination, const char *source, unsigned size);
#include <string.h>

void func_00126664(char *destination, const struct TextReference *reference)
{
    const struct TextObject *object = reference->object;
    char text[32] = {0};
    if (object->length) {
        func_0018f7f4(text, object->text->text, sizeof(text));
        text[31] = 0;
    }
    memcpy(destination, text, sizeof(text));
}
