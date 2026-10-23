#include "game/font.h"
#include "game/text_writer.h"



extern "C" float func_00216518(const struct TextWriterState *writer,
                    const unsigned short *text, int length)
{
    struct TextWriterState copy;
    struct TextBounds bounds;
    bounds.left = 0.0f;
    bounds.top = 0.0f;
    bounds.right = 0.0f;
    bounds.bottom = 0.0f;
    func_00192720(&copy, writer, 100);
    copy.cursor_x = 0;
    copy.cursor_y = 0;
    func_0016471c(&copy, &bounds, &text, length);
    {
        float width = bounds.right - bounds.left;
        /* [INFERENCE] callee identity: inlined by armlink. */
        func_00174480(&copy);
        return width;
    }
}
