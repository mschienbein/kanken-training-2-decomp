#include "game/font_resource.h"
/* [INFERENCE] Registered copy callback: the first word is an unsigned value;
 * a null output deliberately suppresses the copy. */
void func_0012cebc(const struct WordProperty *property, unsigned *output)
{
    if (output)
        *output = property->value;
}
