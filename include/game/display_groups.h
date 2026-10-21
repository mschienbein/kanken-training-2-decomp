#ifndef GAME_DISPLAY_GROUPS_H
#define GAME_DISPLAY_GROUPS_H

struct W17SmallCollection;
struct W6DisplayCollection;
struct W17WideHandle;
struct W17WideCollection;
struct W18DisplayCollection;
struct L044TrainingFrame;
struct W17Panel1068;
struct W17DisplayGroup;
struct W17Panel1578;
struct W17Panel16e8;
struct W17Panel1dc4;
struct W17Panel395c;
struct W17Panel43f0;

#include "game/screen.h"
#include "game/pane_descriptors.h"
#include "game/command_state.h"
#include "game/resource_runtime.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */






struct W17SmallCollection {
    DisplayHandle *handles[9];
};
struct W6DisplayCollection {
    unsigned char reserved00[0x18];
    struct DisplayObject **objects[1];
};
struct W17WideHandle {
    unsigned int reserved00;
    DisplayObject *object;
    DisplayObject *get_object() { return object; }
};
struct W17WideCollection {
    unsigned int reserved00;
    W17WideHandle *handles[11];
    W17WideHandle *at(int index) { return handles[index]; }
};
struct W18DisplayCollection {
    DisplayHandle *handles[10];
};
struct L044TrainingFrame {
    DisplayHandle *handles[27];
};

extern "C" {







}
#endif

#endif
