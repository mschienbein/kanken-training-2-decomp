#include "game/audio_voice.h"
#include "game/service_runtime.h"
#include "game/stream_manager.h"

struct ServiceCoordinator;

extern void func_00182210(void);
extern int func_0018f018(void);

extern void func_00182204(void);

int func_0014877c(struct ServiceOutput *state, int mode)
{
    int result;
    func_00182210();
    if (func_0018f018()) {
        state->output_mode = (unsigned char)mode;
        result = func_00148988(&bss_004b5284, mode);
        func_00182204();
        return result;
    }
    func_00182204();
    return 0;
}
