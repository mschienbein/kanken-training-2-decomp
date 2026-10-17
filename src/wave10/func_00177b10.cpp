class CallbackTarget;
typedef void (CallbackTarget::*MemberCallback)();

struct DeferredMemberCallback {
    CallbackTarget *object;
    MemberCallback method;
};

/* INTEGRATOR: move to include/game/deferred_callback.h */
extern "C" {
extern struct DeferredMemberCallback data_0023efb0;
extern void func_001b7dac(void);
extern void func_0015dec4(void (*callback)(void));
}

extern "C" void func_00177b10(CallbackTarget *object, MemberCallback method)
{
    data_0023efb0.object = object;
    data_0023efb0.method = method;
    func_0015dec4(func_001b7dac);
}
