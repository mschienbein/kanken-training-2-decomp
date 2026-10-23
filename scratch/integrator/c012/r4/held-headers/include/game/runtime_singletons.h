#ifndef GAME_RUNTIME_SINGLETONS_H
#define GAME_RUNTIME_SINGLETONS_H
struct ServiceSingletonW22C;
#ifdef __cplusplus
#include "game/render_parameter.h"
/* Names-admission proves these native references and registrations.
 * [INFERENCE] Descriptive singleton/storage identities, not original names. */
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

class ServiceSingletonW22C {
public:
    ServiceSingletonW22C() {}
    /* The vtable and initializer prove a receiver-bearing cleanup entry. */
    virtual ~ServiceSingletonW22C();
};

extern "C" {
SingletonHolderW22D *func_001c1c74(void);
ServiceSingletonW22C *func_001ebda4(void);
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
void func_001ebe1c(struct ServiceSingletonW22C *object);
#ifdef __cplusplus
}
#endif
#endif
