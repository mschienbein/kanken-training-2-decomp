#include "game/render_parameter.h"

struct SingletonStorageW22D {
    void *head;
    unsigned int reserved04;
    SingletonStorageW22D() : head(0) {}
};
struct SingletonHolderW22D {
    SingletonStorageW22D *instance;
    SingletonHolderW22D()
    {
        if (instance == 0)
            instance = new (data_002232d5) SingletonStorageW22D;
    }
    ~SingletonHolderW22D();
};

/* INTEGRATOR: move to include/game/runtime.h */
extern "C" {
SingletonHolderW22D *func_001c1c74(void);
}

SingletonHolderW22D *func_001c1c74(void)
{
    /* [INFERENCE] callee identities: static destruction registration and
     * guard release were inlined by armlink; erased destinations are unrecoverable. */
    static SingletonHolderW22D holder_001c1c74;
    return &holder_001c1c74;
}
