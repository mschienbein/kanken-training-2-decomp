#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/prompt.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/text_writer.h"
#include "game/runtime.h"
#include "game/screen.h"

struct SelectionGrid24;
struct SelectionGrid29;
struct PairedDisplayOwner;

struct ScreenCleanupBase {
    const void *vtable;
    unsigned char reserved04[0x30];
    int restore_id;
    unsigned char reserved38[8];
    void *components40[4];
    void *motion_trigger;
    SelectionGrid24 *grid24;
    void *components58[2];
    SelectionGrid29 *grid29;
    void *components64[11];
};

struct MotionCleanupScreen {
    ScreenCleanupBase base;
    unsigned char reserved90[0x10];
    DisplayHandle **six_handles;
    DisplayHandle **four_handles;
    MotionState motion;
};

struct GridCleanupScreen {
    ScreenCleanupBase base;
    unsigned char reserved90[0x14];
    DisplayHandle **six_handles;
    DisplayHandle **four_handles;
};

/* INTEGRATOR: move to include/game/selection_cleanup.h */
extern "C" {





void func_00168618(DisplayHandle **handles);
void func_001685c0(DisplayHandle **handles);
void func_00168454(SelectionGrid24 *grid);
void func_001682e8(SelectionGrid29 *grid);



void func_001c1600(PairedDisplayOwner *owner);
extern const unsigned data_002237b4[];
extern const unsigned data_00223890[];
}

extern "C" void func_001b6db4(ScreenCleanupBase *screen)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_001c1600(func_0016e270());
        func_00176e24(screen);
        func_00176df4();
        func_00176dc8(screen->restore_id);
    }
}
