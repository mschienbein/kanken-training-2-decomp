#include "game/font.h"
#include "game/font_resource.h"
#include "game/runtime.h"



void func_001c5b8c(FontResource *font)
{
    FontFile *file = font->file;
    font->vtable = &data_00223d98;
    if (file)
        func_0013ea40(font);
    func_001852f4(func_001c5b84(font));
}
