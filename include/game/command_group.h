#ifndef GAME_COMMAND_GROUP_H
#define GAME_COMMAND_GROUP_H

struct ArchiveReader;
struct AudioChannelState;
struct AudioParameterState;
struct AudioTransitionState;
struct InputRegistrationState;
struct OptionalVoiceMode;
struct PendingTask128;
struct PendingTask68;
struct PoolLink;
struct PoolList;
struct ReadResultState;
struct SignalState;
struct TaskQueueContext;
struct TaskQueueNode;
struct TouchInputService;
struct VoiceByteParameters140;
struct VoiceBytes4;

#include "game/pool_list.h"
#include "game/runtime.h"
#include "game/synchronization.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ReadResultState {
    void *buffer;
    unsigned int size;
    unsigned char reserved08[0x40];
    unsigned short count;
    unsigned short flags;
};

struct VoiceBytes4 {
    unsigned char values[4];
};

struct VoiceByteParameters140 {
    unsigned char reserved00[0x140];
};

struct AudioTransitionState {
    unsigned char reserved00[0xfe];
    unsigned char mode;
    unsigned char reservedff[9];
    float value;
};

struct PendingTask68 {
    unsigned char reserved00[0x0c];
    struct SignalState completion;
    unsigned char reserved14[0x10];
    struct TaskQueueContext *context;
    unsigned char reserved28[0x14];
    struct TaskQueueNode *queued;
    unsigned char reserved40[4];
    struct PoolLink link;
};

struct PendingTask128 {
    unsigned char reserved00[0x0c];
    struct SignalState completion;
    unsigned char reserved14[0x10];
    struct TaskQueueContext *context;
    unsigned char reserved28[0x50];
    struct TaskQueueNode *queued;
    unsigned char reserved7c[4];
    struct PoolLink link;
};

struct AudioParameterState {
    unsigned char reserved00[0xe8];
    float value;
};

struct OptionalVoiceMode {
    unsigned char reserved00[0x9c];
    unsigned char active;
    unsigned char reserved9d[0xcc];
    signed char mode;
};

struct MainThreadRecord {
    struct InputRegistrationState *registration;
    unsigned char *thread_local_storage;
};

#ifdef __cplusplus
/* [INFERENCE] This serialized member table supplies native callback records.
 * Keep each callback's actual arity, width and signedness; the receiver remains
 * opaque rather than being equated with an unrelated padded sound class. */
class CommandTarget;
struct CommandCallbacks {
    void (CommandTarget::*mode)(unsigned char);
    void (CommandTarget::*fade)(signed char, int);
    void (CommandTarget::*value08)(float);
    void (CommandTarget::*value0c)(float);
    void (CommandTarget::*value10)(float);
    void (CommandTarget::*value14)(float);
    void (CommandTarget::*value104)(float);
    void (CommandTarget::*value108)(float, int);
    void (CommandTarget::*value18)(float);
    void (CommandTarget::*flag83)(unsigned char);
    void (CommandTarget::*flagf4)(signed char);
    void (CommandTarget::*flagf0)(unsigned char);
};
struct CommandGate {
    unsigned values[2];
    unsigned char enabled;
    unsigned char reserved[3];
};
struct CommandGroup {
    unsigned char reserved0[0x90];
    CommandTarget *children[16];
    unsigned char reserved1[0x134 - 0xd0];
    CommandGate gates[4];
    CommandTarget *child(int index) const
    {
        /* ORIGINAL BUG: negative indices are not rejected. */
        if (index > 15) return 0;
        return children[index];
    }
};
extern "C" {
extern const CommandCallbacks data_0021df3c;
void func_0015121c(CommandGroup *group, unsigned mask, unsigned char value);
void func_00151458(CommandGroup *group, unsigned mask, signed char value);
void func_00151584(CommandGroup *group, unsigned mask, unsigned char value);
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern struct InputRegistrationState bss_004b4fc0;
extern void func_00113630(struct InputRegistrationState *state);
extern struct MainThreadRecord data_00242d78;
extern struct TouchInputService bss_004b1fb8;
extern int func_0019ec14(struct SignalState *state);
extern struct PoolLink *func_0016a0d0(struct TaskQueueContext *context,
                                     struct TaskQueueNode *node);
extern void func_00141560(unsigned index, float value);
extern void func_0010bec8(void);
extern struct TouchInputService *func_0010c12c(void);
extern void func_001950ec(struct ReadResultState *state);
extern void func_001c540c(struct ArchiveReader *reader);
extern void func_001c5958(struct AudioChannelState *state, unsigned int mode);
extern void func_001cf1dc(struct VoiceByteParameters140 *voice,
                              unsigned int index, unsigned char value);
extern void func_001d4d48(struct AudioTransitionState *state, float value,
                              unsigned char mode);
extern int func_001d57b0(struct PendingTask68 *task);
extern void func_001d57cc(struct PendingTask68 *task);
extern int func_001d85d8(struct PendingTask128 *task);
extern void func_001d85f4(struct PendingTask128 *task);
extern void func_001db0f8(struct AudioParameterState *state, float value);
extern int func_001f1890(const struct OptionalVoiceMode *voice);
extern void func_002161ec(struct PoolList *list);
#ifdef __cplusplus
}
#endif

#endif
