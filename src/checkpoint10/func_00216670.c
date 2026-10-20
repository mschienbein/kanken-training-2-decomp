#include "game/runtime.h"
#include "game/text_writer.h"
#include "game/functions.h"
#include "game/screen.h"

struct W24eTextWriter {
    unsigned reserved00[11];
    struct WidgetPosition position;
    unsigned char reserved34[0x30];
};

extern float func_002166cc(struct W24eTextWriter *writer,
                           const unsigned short *text, int length);

float func_00216670(struct W24eTextWriter *writer,
                    const unsigned short *text, int length)
{
    struct W24eTextWriter copy;
    float width;
    func_00192720(&copy, writer, sizeof(copy));
    width = func_002166cc(&copy, text, length);
    writer->position = copy.position;
    /* [INFERENCE] callee identity: inlined by armlink */
    func_00125b54(&copy);
    return width;
}
