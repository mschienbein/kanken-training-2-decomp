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
struct MenuRowManager;
struct PagedMenuSceneB : MenuSceneBase {
    unsigned char reserved90[0x440];
    MenuRowManager *rows;
};

/* INTEGRATOR: move to include/game/menu_scene.h */
extern "C" {







void func_0016d394(MenuRowManager *rows);


void func_00177160(MenuChannelB *channel);
void func_001bd968(PagedMenuSceneB *scene);
extern const unsigned data_002236f4[];
}

static __forceinline void release_paged_scene(PagedMenuSceneB *scene)
{
    if (!func_00177124(7)) {
        func_00177084();
        scene->next_scene = 4;
        func_0016d394(scene->rows);
        if (scene->panel) {
            func_0016d4f8(scene->panel);
            func_0016d310(scene->panel);
        }
        func_00177160(scene->channel_b);
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->next_scene);
    }
}

void func_001bd968(PagedMenuSceneB *scene)
{
    scene->vtable = data_002236f4;
    release_paged_scene(scene);
    func_00174480(scene);
}
