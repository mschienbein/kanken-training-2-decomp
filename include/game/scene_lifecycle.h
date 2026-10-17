#ifndef GAME_SCENE_LIFECYCLE_H
#define GAME_SCENE_LIFECYCLE_H

struct AudioChannel;
struct AudioVoiceGroup;
struct PoolLink;
struct PoolList;
struct SceneAggregate;
struct SceneBase;
struct SceneController;
struct SceneGroup;
struct SoundPlaybackState;
struct ObjectPool;
struct SceneLifecycleVTable;
struct SceneLifecycle;
struct W17SceneBase;
struct W17CompositeScene;
struct W17ExtendedScene;
struct W18SceneA;
struct W17SceneVTable;
struct W18SceneB;
struct W18SceneC;
struct SceneToggle;
struct W17SmallCollection;
struct W6DisplayCollection;
struct W17WideCollection;
struct W18DisplayCollection;
struct W18WideDisplayCollection;
struct L045Scene0;
struct L045Scene1;
struct L045Scene2;
struct L045Scene3;
struct L045Scene4;
struct L045Scene5;
struct L045Scene6;
struct L045Scene7;
struct L045Scene8;
struct L045Scene9;
struct L045Scene10;
struct L045Scene11;
struct L045Scene12;
struct L045Scene13;
struct L045Scene14;
struct L045Scene15;
struct L045Scene16;
struct L045Scene17;
struct L045Scene18;
struct L045Scene19;
struct L045Scene20;

#include "game/notification.h"










#include "game/pool_list.h"
#include "game/audio_voice.h"
#include "game/sound_track.h"
#include "game/sound_runtime.h"

#include <stdbool.h>
#include "game/functions.h"



struct SceneEngine {
    const void *vtable;
    unsigned char reserved04[0x4d];
    unsigned char inhibited;
    unsigned char reserved52[0x32];
    struct PoolList active;
#ifdef __cplusplus
    ~SceneEngine();
#endif
};


struct ResourceCallbackState { unsigned state, completion; };

#ifdef __cplusplus
extern "C" {
#endif
extern struct AudioVoiceGroup *func_00138cb0(struct AudioVoiceGroup *resource);
extern void func_00152f10(struct AudioVoiceGroup *resource, int event, void *context);
extern void func_0018290c(struct AudioVoiceGroup *resource, int event, void *context);

extern struct ObjectPool *func_00185cc0(void);
extern void func_00182994(struct ObjectPool *pool, void *object);
extern void func_001829d0(struct AudioChannel *channel);
extern void func_00183550(struct AudioVoiceGroup *group);
extern void func_0016a27c(struct AudioVoiceGroup *resource, int enabled);
extern void func_001d4cf4(struct SoundTrack *parent, bool enabled);
#ifdef __cplusplus
}
#endif


struct SceneGroup {
    unsigned char reserved00[0xe];
    unsigned char enabled;
    unsigned char reserved0f[0x90 - 0xf];
    struct SoundTrack *children[16];
};

#ifdef __cplusplus
struct SceneBase : SoundPlaybackState {
    ~SceneBase()
    {
        vtable = data_0022459c;
        active = 3;
    }
};
#endif

#ifdef __cplusplus
struct SceneController {
    const void *vtable;
    /* Member extent follows from the engine at aggregate offset 0x2314;
     * no internal controller-record capacity is assumed. */
    unsigned char reserved04[0x21fc - 4];
    ~SceneController();
};
#endif

#ifdef __cplusplus
struct SceneAggregate : SceneBase {
    SceneController controller;
    SceneEngine engine;
    ~SceneAggregate();
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void func_001d8960(struct SceneGroup *group, bool enabled);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */










struct SceneLifecycleVTable {
    unsigned char reserved00[8];
    void (*initialize)(SceneLifecycle *scene);
    unsigned char reserved0c[8];
    void (*finalize)(SceneLifecycle *scene);
};
struct SceneLifecycle {
    const SceneLifecycleVTable *vtable;
};
struct W17SceneBase {
    const W17SceneVTable *vtable;
    unsigned char reserved04[0x30];
    int index;
};
struct W17CompositeScene {
    W17SceneBase base;
    unsigned char reserved38[0x1e8];
    W17SmallCollection *small;
    W6DisplayCollection *display;
    W17WideCollection *wide;
};
struct W17ExtendedScene {
    W17SceneBase base;
    unsigned char reserved38[0x58];
    int selection;
};
struct W18SceneA {
    W17SceneBase base;
    unsigned char reserved38[0x7c];
    W18DisplayCollection *objects;
};
struct W17SceneVTable {
    unsigned char reserved00[0x18];
    void (*prepare)(W17SceneBase *scene);
    unsigned int reserved1c;
    void (*finish)(W17SceneBase *scene);
};
struct W18SceneB {
    W17SceneBase base;
    unsigned char reserved38[0x74];
    W18DisplayCollection *objects;
};
struct W18SceneC {
    W17SceneBase base;
    unsigned char reserved38[0x144];
    W18WideDisplayCollection *objects;
};
struct SceneToggle {
    unsigned char reserved00[14];
    bool enabled;
    unsigned char reserved0f[0x8d];
    struct SceneChild *child;
};

extern "C" {

extern SceneLifecycle bss_0028e900;
extern SceneLifecycle bss_0028d9a4;
extern SceneLifecycle bss_0028f200;
extern SceneLifecycle bss_0028fdd4;
extern SceneLifecycle bss_0028fe70;
extern SceneLifecycle bss_0028ff10;
extern SceneLifecycle bss_0028da50;
extern SceneLifecycle bss_0028e1e0;
extern SceneLifecycle bss_0028f080;
extern SceneLifecycle bss_0028ecf4;
extern SceneLifecycle bss_0028e318;
extern SceneLifecycle bss_0028e790;
extern SceneLifecycle bss_0028e404;




int func_00177124(int mode);
void func_00177084(void);
void func_001c1abc(W17SmallCollection *collection);
void func_001c29ac(W6DisplayCollection *collection);
void func_001c2d74(W17WideCollection *collection);
void func_00181b58(int enabled);
int func_0016f9c0(unsigned int mask);
void func_001dc240(unsigned int mask);
void func_00170f20(int index);
TransformEntry *func_00174454(int index, int part);
void func_00176e24(W17SceneBase *scene);
void func_00176df4(void);
void func_00176dc8(int index);
extern const W17SceneVTable data_0022360c;
void func_00174480(void *object);
extern const W17SceneVTable data_0022380c;
extern const W17SceneVTable data_00223838;
extern const W17SceneVTable data_00223864;
extern const W17SceneVTable data_00223908;

int func_00171e30(void);
void func_00181b58(unsigned short screen);
void func_00170f6c(W18DisplayCollection *collection);
int func_001c20e8(W18DisplayCollection *collection);
extern const W17SceneVTable data_002235c8;
extern const W17SceneVTable data_00223720;

void func_001c13f4(W18WideDisplayCollection *collection);
extern const W17SceneVTable data_00223934;





















extern void func_00203810(void);
extern void func_00164ebc(void *object, ObjectDestructor destroy, void (*image_base)(void));
extern L045Scene0 bss_0028d9a4;
extern L045Scene0 *func_001e3ea4(L045Scene0 *object);
extern void func_001e3ec8(L045Scene0 *object);
extern L045Scene1 bss_0028da50;
extern L045Scene1 *func_001e8714(L045Scene1 *object);
extern void func_001e8738(L045Scene1 *object);
extern L045Scene2 bss_0028db2c;
extern L045Scene2 *func_001bb8c0(L045Scene2 *object);
extern void func_001bb8e4(L045Scene2 *object);
extern L045Scene3 bss_0028dd7c;
extern L045Scene3 *func_001c0d30(L045Scene3 *object);
extern void func_001c0d54(L045Scene3 *object);
extern L045Scene4 bss_0028dfa4;
extern L045Scene4 *func_001b325c(L045Scene4 *object);
extern void func_001b3280(L045Scene4 *object);
extern L045Scene5 bss_0028e1e0;
extern L045Scene5 *func_001ec128(L045Scene5 *object);
extern void func_001ec14c(L045Scene5 *object);
extern L045Scene6 bss_0028e27c;
extern L045Scene6 *func_001b7294(L045Scene6 *object);
extern void func_001b72b8(L045Scene6 *object);
extern L045Scene7 bss_0028e318;
extern L045Scene7 *func_001a29f4(L045Scene7 *object);
extern void func_001a2a18(L045Scene7 *object);
extern L045Scene8 bss_0028e404;
extern L045Scene8 *func_001ab51c(L045Scene8 *object);
extern void func_001ab540(L045Scene8 *object);
extern L045Scene9 bss_0028e790;
extern L045Scene9 *func_001a7420(L045Scene9 *object);
extern void func_001a7444(L045Scene9 *object);
extern L045Scene10 bss_0028e848;
extern L045Scene10 *func_001acb90(L045Scene10 *object);
extern void func_001acbb4(L045Scene10 *object);
extern L045Scene11 bss_0028e900;
extern L045Scene11 *func_001e3270(L045Scene11 *object);
extern void func_001e3294(L045Scene11 *object);
extern L045Scene12 bss_0028ecf4;
extern L045Scene12 *func_00162f78(L045Scene12 *object);
extern void func_00162f9c(L045Scene12 *object);
extern L045Scene13 bss_0028f080;
extern L045Scene13 *func_001ef790(L045Scene13 *object);
extern void func_001ef7b4(L045Scene13 *object);
extern L045Scene14 bss_0028f200;
extern L045Scene14 *func_001e5658(L045Scene14 *object);
extern void func_001e567c(L045Scene14 *object);
extern L045Scene15 bss_0028f3d8;
extern L045Scene15 *func_001ba1d4(L045Scene15 *object);
extern void func_001ba1f8(L045Scene15 *object);
extern L045Scene16 bss_0028f850;
extern L045Scene16 *func_001bd944(L045Scene16 *object);
extern void func_001bd968(L045Scene16 *object);
extern L045Scene17 bss_0028fd24;
extern L045Scene17 *func_001be104(L045Scene17 *object);
extern void func_001be128(L045Scene17 *object);
extern L045Scene18 bss_0028fdd4;
extern L045Scene18 *func_001e744c(L045Scene18 *object);
extern void func_001e7470(L045Scene18 *object);
extern L045Scene19 bss_0028fe70;
extern L045Scene19 *func_001e7c4c(L045Scene19 *object);
extern void func_001e7c70(L045Scene19 *object);
extern L045Scene20 bss_0028ff10;
extern L045Scene20 *func_001e7fb8(L045Scene20 *object);
extern void func_001e7fdc(L045Scene20 *object);
void func_001d5c50(struct SceneToggle *toggle, bool enabled);
}
#endif

#endif
