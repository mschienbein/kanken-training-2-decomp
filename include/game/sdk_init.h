#ifndef GAME_SDK_INIT_H
#define GAME_SDK_INIT_H

struct Result;
struct WaitableHandle;

#include "ctr/svc.h"
#include "game/waitable_handle.h"
struct KernelConfiguration { unsigned char reserved00[0x14]; unsigned char environment; };
struct TranslationOffsets { unsigned first, second; };
struct FatalErrorInfo {
    unsigned char type, reserved;
    unsigned short version;
    unsigned result, address, handle;
    unsigned char reserved10[0x70];
};
#ifdef __cplusplus
extern "C" {
#endif
extern signed char data_00242d64;
extern void func_001135e8(void);
extern const CtrHandle data_0021c110, data_00220b8c;
extern struct TranslationOffsets data_00243798;
extern struct FatalErrorInfo bss_004b1200;
extern void func_00106f00(unsigned size);
extern void func_00106fbc(unsigned size);
extern int func_0019eef0(unsigned size);
extern void func_001219bc(unsigned result, unsigned char type, unsigned address);
extern void func_00186294(struct FatalErrorInfo *info);
extern CtrResult func_001565b0(CtrHandle *duplicate, CtrHandle original);
extern CtrResult func_0011ef2c(long long *value, CtrHandle process, CtrProcessInfoType type);
#ifdef __cplusplus
}
#include "game/result.h"
struct OwnedHandle {
    WaitableHandle state;
    OwnedHandle()
    {
        CtrHandle duplicate;
        state.handle = 0;
        duplicate = 0;
        Result result(func_001565b0(&duplicate, data_0021c110));
        switch (result.failed()) {
        case 0: break;
        default: func_0019dfbc(result.value); break;
        }
        state.handle = duplicate;
        state.waited = 0;
        state.attached = 0;
    }
    ~OwnedHandle();
};
#endif



#endif
