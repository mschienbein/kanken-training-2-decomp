#ifndef GAME_UI_RESOURCE_H
#define GAME_UI_RESOURCE_H
#include <preinclude.h>
#include <nw/gfx/gfx_GfxObject.h>
#include <nw/ut/ut_MoveArray.h>

/* [INFERENCE] Descriptive context identity. Ownership and destruction use
 * canonical GfxPtr and MoveArray types, not byte-offset lifetime surrogates. */
struct UiContextVTable;
struct UiArrayRegion {
    unsigned reserved40[2];
    nw::ut::MoveArray<unsigned> first_array;
    unsigned char reserved58[8];
    nw::ut::MoveArray<unsigned> second_array;
    unsigned char reserved70[0x38];
    nw::ut::MoveArray<unsigned> third_array;
    unsigned reservedb8;
    nw::ut::MoveArray<unsigned> fourth_array;
    nw::ut::MoveArray<unsigned> fifth_array;
};
struct UiResourceContext {
    const UiContextVTable *vtable;
    unsigned reserved04;
    nw::gfx::GfxPtr<nw::gfx::GfxObject> first;
    unsigned char reserved10[0x30];
    UiArrayRegion arrays;
    unsigned char reserveddc[0x50];
    nw::gfx::GfxPtr<nw::gfx::GfxObject> last;
    ~UiResourceContext();
};
extern "C" { extern const UiContextVTable data_00223e78; }
#endif
