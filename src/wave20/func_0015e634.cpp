#include "game/screen.h"

struct SurfaceSize {
    float width, height;
    SurfaceSize(float w, float h) : width(w), height(h) {}
    SurfaceSize(const SurfaceSize &other) : width(other.width), height(other.height) {}
    bool operator==(const SurfaceSize &other) const
    { return width == other.width && height == other.height; }
    bool operator!=(const SurfaceSize &other) const { return !(*this == other); }
};
struct SurfaceTextState {
    unsigned char reserved00[0xe4];
    SurfaceSize size;
    float line_spacing, character_spacing;
    unsigned char reservedf4[0x0a];
    unsigned char dirty;
    bool update_dirty(bool changed) { dirty |= changed; return changed; }
    void set_size(const SurfaceSize &value)
    {
        if (update_dirty(!(size == value)))
            size = value;
    }
    void set_character_spacing(float value)
    {
        if (update_dirty(character_spacing != value))
            character_spacing = value;
    }
    void set_line_spacing(float value)
    {
        if (update_dirty(line_spacing != value))
            line_spacing = value;
    }
};
struct DisplaySurface {
    unsigned char reserved00[8];
    SurfaceTextState *text;
};

extern "C" void func_0015e634(DisplaySurface *surface, float width, float height)
{
    surface->text->set_size(SurfaceSize(width, height));
}
