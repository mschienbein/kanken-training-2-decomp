#ifndef GAME_TEXT_RENDERER_H
#define GAME_TEXT_RENDERER_H

struct RecordSet;

#include "game/runtime.h"

#ifdef __cplusplus
#include <nw/ut/ut_Color.h>
/* [INFERENCE] Shared target-version text-field view used by application
 * wrappers; not the complete native TextBox or a material-bearing pane. */
struct SurfaceSize {
    float width, height;
    SurfaceSize(float w, float h) : width(w), height(h) {}
    SurfaceSize(const SurfaceSize &other) : width(other.width), height(other.height) {}
    bool operator==(const SurfaceSize &other) const
    { return width == other.width && height == other.height; }
    bool operator!=(const SurfaceSize &other) const { return !(*this == other); }
};
struct SurfaceTextState {
    unsigned char reserved00[0xd8];
    nw::ut::Color8 colors[2];
    unsigned char reservede0[4];
    SurfaceSize size;
    float line_spacing, character_spacing;
    unsigned char reservedf4[0x0a];
    unsigned char dirty;
    void SetTextColor(unsigned index, nw::ut::Color8 value)
    {
        colors[index] = value;
    }
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
#endif

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
extern "C" {
#endif
extern void func_00173748(int secondary, int category, int index, int *value, int *extra);
extern void func_001dbcb0(int category, int kind, int index, int *value);
extern int func_001e33dc(const struct RecordSet *set, int index);
extern const void *func_001e33f8(const struct RecordSet *set, int index);
extern const unsigned short *func_001e3468(const struct RecordSet *set, int index);
extern int func_001e3490(const struct RecordSet *set, int index);
extern const void *func_001e34ac(const struct RecordSet *set, int index);
extern const void *func_001e34d4(const struct RecordSet *set, int index);
extern const void *func_001e34fc(const struct RecordSet *set, int index);
extern const unsigned short *func_001e3524(const struct RecordSet *set, int index);
#ifdef __cplusplus
}
#endif

#endif
