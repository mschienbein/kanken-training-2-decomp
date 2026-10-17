/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/font/font_TagProcessorBase.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../types.h"
#include "../ut/ut_Rect.h"

namespace nw {
namespace font {

template <typename CharType>
class TextWriterBase;

template <typename CharType>
struct PrintContext
{
    TextWriterBase<CharType>* writer;
    const CharType* str;
    const f32 xOrigin;
    const f32 yOrigin;
    u32 flags;

    PrintContext(TextWriterBase<CharType>* aWriter, const CharType* aStr, const f32 aXOrigin, const f32 aYOrigin, u32 aFlags):
        writer(aWriter),
        str(aStr),
        xOrigin(aXOrigin),
        yOrigin(aYOrigin),
        flags(aFlags) {}
};

template <typename CharType>
class TagProcessorBase
{
public:
    enum Operation
    {
        OPERATION_DEFAULT,
        OPERATION_NO_CHAR_SPACE,
        OPERATION_CHAR_SPACE,
        OPERATION_NEXT_LINE,
        OPERATION_END_DRAW,
        NUM_OF_OPERATION
    };

    TagProcessorBase();
    virtual ~TagProcessorBase();

    virtual Operation Process(u16 code, PrintContext<CharType>* pContext);
    virtual Operation CalcRect(ut::Rect* pRect, u16 code, PrintContext<CharType>* pContext);

private:
    typedef PrintContext<CharType> ContextType;

    void ProcessLinefeed(ContextType* pContext);
    void ProcessTab(ContextType* pContext);
};

}
}

