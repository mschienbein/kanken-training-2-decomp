#include "game/text_writer.h"

extern "C" float func_0017a690(const nw::font::WideTextWriter *writer)
{
    int lineFeed = writer->GetFont() ? writer->GetFont()->GetLineFeed() : 0;
    return lineFeed * writer->GetScaleV() + writer->GetLineSpace();
}
