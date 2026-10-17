#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/text_writer.h"
/* [INFERENCE] Recovered partial menu-scene layout; names describe observed roles. */
struct MenuScenePanel;
struct MenuChannelA;
struct MenuChannelB;
struct MenuSceneBase {
    const void *vtable;
    unsigned char reserved04[0x10];
    MenuScenePanel *panel;
    unsigned char reserved18[0x1c];
    int next_scene;
    MenuChannelA *channel_a;
    MenuChannelB *channel_b;
    unsigned char reserved40[0x50];
};

/* INTEGRATOR: move to include/game/menu_scene.h */
extern "C" {







extern const unsigned data_00223570[];
void func_001a7444(MenuSceneBase *scene);
}

void func_001a7444(MenuSceneBase *scene)
{
    scene->vtable = data_00223570;
    if (!func_00177124(7)) {
        func_00177084();
        func_00181b58(4);
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->next_scene);
    }
    func_00174480(scene);
}
