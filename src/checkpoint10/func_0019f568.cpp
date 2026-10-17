#include <stdarg.h>

typedef void (*FatalCallbackW9J)(const char *file, int line, const char *function,
                                 const char *condition, const char *format,
                                 va_list arguments);

/* INTEGRATOR: move to include/game/runtime.h */
extern "C" {
extern FatalCallbackW9J data_00242af8;
void func_00134e7c(unsigned reason, const void *info, unsigned size);
}

/* INTEGRATOR: differs from include/game/runtime.h (void): the body forwards the
 * incoming r1 unchanged as the callback's final stack argument, so it takes a
 * second word. The 73 callers never set it (ORIGINAL BUG: the va_list is whatever
 * r1 held); they keep the no-argument prototype and must not include this
 * definition's declaration. [INFERENCE] the first word (r0) is unused: it is
 * overwritten by the null text before any read; its type is unknown. */
extern "C" void func_0019f568(const void *unused, va_list arguments)
{
    const char *null_text = 0;
    FatalCallbackW9J callback = data_00242af8;
    if (callback != 0) {
        data_00242af8 = 0;
        callback(null_text, 0, null_text, null_text, null_text, arguments);
    }
    /* [INFERENCE] callee is svc 0x3c (Break); r0 reason, r1/r2 pass zero. */
    func_00134e7c((unsigned)null_text, 0, 0);
}
