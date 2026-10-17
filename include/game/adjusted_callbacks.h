#ifndef GAME_ADJUSTED_CALLBACKS_H
#define GAME_ADJUSTED_CALLBACKS_H

#include <stddef.h>

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct CallbackBase;
struct CallbackPrefix80;
struct CallbackHost80;
struct CallbackPrefix68;
struct CallbackHost68;

struct CallbackBase {
    const void *vtable;
};
struct CallbackPrefix80 {
    unsigned reserved00[20];
};
struct CallbackHost80 {
    CallbackPrefix80 prefix;
    CallbackBase callback;
};
struct CallbackPrefix68 {
    unsigned reserved00[17];
};
struct CallbackHost68 {
    CallbackPrefix68 prefix;
    CallbackBase callback;
};

extern "C" {
void func_001d5ca0(CallbackHost80 *object);
void func_001d786c(CallbackHost68 *object);
void func_001d80b0(CallbackHost68 *object);
void func_001d89e4(CallbackHost80 *object);
}
#endif

#endif
