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

struct ObjectPool;
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

#endif
