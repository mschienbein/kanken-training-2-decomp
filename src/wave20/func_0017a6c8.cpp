#include "game/text_writer.h"

extern "C" float func_0017a6c8(const nw::font::WideTextWriter *writer)
{
    return writer->m_pFont->GetHeight() * writer->m_Scale.y;
}
