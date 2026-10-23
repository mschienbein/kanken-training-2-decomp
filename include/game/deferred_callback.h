#ifndef GAME_DEFERRED_CALLBACK_H
#define GAME_DEFERRED_CALLBACK_H

#ifdef __cplusplus
/* [INFERENCE] Descriptive virtual names; slots 4 and 5 are used by member callbacks. */
class CallbackTarget {
public:
    virtual void callback0();
    virtual void callback1();
    virtual void callback2();
    virtual void callback3();
    virtual void callback4();
    virtual void callback5();
    virtual void callback6();
    virtual void callback7();
    virtual void callback8();
};
typedef void (CallbackTarget::*MemberCallback)();

struct DeferredMemberCallback {
    CallbackTarget *object;
    MemberCallback method;
};

extern "C" {
extern const MemberCallback data_002188d8;
extern struct DeferredMemberCallback data_0023efb0;
extern void func_001b7dac(void);
extern void func_0015dec4(void (*callback)(void));
extern void func_00177b10(CallbackTarget *object, MemberCallback method);
}
#endif


#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */




extern "C" {
extern MemberCallback data_0023efb4;
}
#endif

#endif
