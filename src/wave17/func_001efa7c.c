struct TextRange {
    const unsigned short *begin;
    const unsigned short *end;
};

struct TextRangeResult {
    unsigned length;
    const unsigned short *begin;
};

unsigned func_001efa7c(const struct TextRange *text, struct TextRangeResult *out)
{
    unsigned length = text->end - text->begin;
    if (length > 256)
        return 0xe0e046c8u;
    out->length = length;
    out->begin = text->begin;
    return 0;
}
