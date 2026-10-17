#include "ctr/svc.h"
/* INTEGRATOR: reconcile the session pointer and source/size fields with
 * include/game/service_runtime.h. This caller sign-extends the mode at +3,
 * contrary to src/sdk/func_00136878.cpp's unsigned-byte declaration. */
struct ServicePublicationState {
    unsigned char deferred, published, pending;
    signed char acquired;
    unsigned short first, second;
    const CtrHandle *session;
    unsigned busy;
    unsigned char lock_storage[8];
    CtrHandle handle;
    const void *source;
    unsigned size;
};
extern struct ServicePublicationState data_00242afc;
/* [INFERENCE] Dispatch sets up no explicit callback arguments; callback
 * identities and their complete contracts remain to be adjudicated. */
extern void (*bss_0049eee4[8])(void);
extern CtrResult func_00185050(const void *source, unsigned size,
    unsigned short first, unsigned short second);
extern void func_00192afc(CtrResult result, const void *location);
/* [INFERENCE] Signed-byte mode matches this caller's extension. The callee
 * body does not establish whether its source parameter was signed char or int. */
extern CtrResult func_00136878(const CtrHandle *session, signed char mode);
static __inline void CheckResult(const void *location, CtrResult result)
{
    switch ((unsigned)result >> 31) {
    case 0: break;
    default: func_00192afc(result, location); break;
    }
}
static CtrResult release_optional_session(const CtrHandle *session,
                                          signed char mode)
{
    if (session)
        return func_00136878(session, mode);
    return 0;
}
CtrResult func_001368b0(void)
{
    CtrResult result;
    int index;
    data_00242afc.deferred = 0;
    if (!data_00242afc.pending)
        return 0;
    result = func_00185050(data_00242afc.source, data_00242afc.size,
        data_00242afc.first, data_00242afc.second);
    CheckResult((const void *)__current_pc(), result);
    for (index = 0; index < 8; ++index)
        if (bss_0049eee4[index])
            bss_0049eee4[index]();
    data_00242afc.pending = 0;
    return release_optional_session(data_00242afc.session, data_00242afc.acquired);
}
