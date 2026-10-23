#ifndef GAME_AUDIO_H
#define GAME_AUDIO_H

struct AnimatedObject;
struct DisplayAnimationTrack;
struct DisplayObject;
struct Handle1c4478;
struct Handle1c4484;

struct PromptManager;
struct SoundStarter;
struct SoundTrack;
struct Widget1ace94;

#include "game/runtime.h"
#include "game/screen.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct AnimatedObject {
    struct DisplayObject base;
#ifndef __cplusplus
    struct DisplayAnimationTrack *tracks[];
#endif
};

struct Widget1ace94 {
    unsigned char reserved00[0x0c];
    struct DisplayObject **objects;
};

struct Handle1c4478 {
    int slot;
};

struct Handle1c4484 {
    int slot;
};



#ifdef __cplusplus
class SoundStarter {
public:
    virtual void reserved_slot0();
    virtual void reserved_slot1();
    virtual unsigned start(SoundTrack **track, unsigned resource,
                           unsigned reserved, unsigned flags);
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern int func_00195874(const struct DisplayObject *object, int index);
extern void func_001d2b80(struct SoundTrack *track);
extern void func_001c1794(struct PromptManager *manager);
extern int func_001c1924(const struct PromptManager *manager);
extern int func_001ace94(const struct Widget1ace94 *widget);
extern struct Handle1c4478 *func_001c4478(struct Handle1c4478 *handle);
extern struct Handle1c4484 *func_001c4484(struct Handle1c4484 *handle);

extern unsigned char func_001d018c(struct SoundStarter *starter, struct SoundTrack **track,
                                     unsigned resource, unsigned flags);
#ifdef __cplusplus
}
#endif

#endif
