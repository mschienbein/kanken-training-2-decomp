#include "game/text_writer.h"

static float include_top(float next, float current) { return (current <= next) ? current : next; }
static float include_right(float next, float current) { return (current >= next) ? current : next; }
static float include_bottom(float next, float current) { return (current >= next) ? current : next; }

static void measure_string_bounds(TextBounds *bounds, TextWriterState *writer,
                             const unsigned short *text, int length)
{
    const unsigned short *cursor = text;
    const unsigned short *end = text + length;
    int remaining = length;
    bounds->left = 0;
    bounds->right = 0;
    bounds->top = 0;
    bounds->bottom = 0;
    writer->cursor_x = 0.0f;
    writer->cursor_y = 0.0f;
    do {
        TextBounds line;
        func_0016471c(writer, &line, &cursor, remaining);
        remaining = end - cursor;
        bounds->left = (bounds->left <= line.left ? bounds->left : line.left);
        bounds->top = include_top(line.top, bounds->top);
        bounds->right = include_right(line.right, bounds->right);
        bounds->bottom = include_bottom(line.bottom, bounds->bottom);
    } while (remaining > 0);
}

extern "C" void func_00216594(TextWriterState *writer, TextBounds *bounds,
                             const unsigned short *text, int length)
{
    measure_string_bounds(bounds, writer, text, length);
}
