#include "game/font.h"

struct FontVTable {
    void (*destroy)(FontResource *font);
    void (*delete_object)(FontResource *font);
    int (*width)(const FontResource *font);
    int (*height)(const FontResource *font);
    unsigned char reserved10[0x38];
    int (*kind)(const FontResource *font);
};

struct FontSize {
    float width, height;
    FontSize(float w, float h) : width(w), height(h) {}
    bool operator==(const FontSize &other) const
    {
        return width == other.width && height == other.height;
    }
};

struct TextFontState {
    unsigned char reserved00[0xe0];
    const FontResource *font;
    FontSize size;
    unsigned char reservedec[0x12];
    unsigned char dirty;
    bool UpdateDirty(bool changed)
    {
        dirty |= changed ? 1 : 0;
        return changed;
    }
    void SetSize(const FontSize &value)
    {
        if (UpdateDirty(!(size == value)))
            size = value;
    }
};

extern "C" void func_001cf83c(TextFontState *state, const FontResource *font)
{
    if (!font || font->vtable->kind(font) == 1) {
        if (state->UpdateDirty(state->font != font)) {
            if ((state->font = font) != 0) {
                state->SetSize(FontSize(
                    static_cast<float>(state->font->vtable->width(state->font)),
                    static_cast<float>(state->font->vtable->height(state->font))));
            } else {
                state->SetSize(FontSize(0.0f, 0.0f));
            }
        }
    }
}
