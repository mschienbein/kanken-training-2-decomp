#ifndef GAME_DEFERRED_CALLBACK_H
#define GAME_DEFERRED_CALLBACK_H

#ifdef __cplusplus
class CallbackTarget;
typedef void (CallbackTarget::*MemberCallback)();

struct DeferredMemberCallback {
    CallbackTarget *object;
    MemberCallback method;
};

extern "C" {
extern struct DeferredMemberCallback data_0023efb0;
extern void func_001b7dac(void);
extern void func_0015dec4(void (*callback)(void));
extern void func_00177b10(CallbackTarget *object, MemberCallback method);
}
#endif

#endif
