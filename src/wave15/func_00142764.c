#include "game/input_runtime.h"
#include "game/audio_voice.h"
#include "game/service_runtime.h"

struct ServiceCoordinator;

struct SystemAudioConfiguration {
    unsigned char reserved00[0xc0];
    signed char audio_value;
};
typedef void (*ServiceCallback)(void);





extern unsigned func_0019edcc(void);


extern void func_00195ea4(struct HandleManager *state);
extern void func_00146070(struct ServiceOutput *state);





extern int func_00145f74(ServiceCallback first, ServiceCallback second,
                        ServiceCallback third);

union ResultStatus {
    unsigned code;
    struct {
        unsigned payload : 31;
        unsigned failure : 1;
    } fields;
};

static __inline unsigned result_failed(unsigned result)
{
    union ResultStatus status;
    status.code = result;
    return status.fields.failure;
}

unsigned func_00142764(void)
{
    unsigned result;
    signed char value;
    if (data_00242d90.initialized)
        return 0x0820abf9;
    result = func_0019edcc();
    if (result_failed(result))
        return result;
    result = func_00142838(&bss_004b5284, 0);
    if (result_failed(result))
        return result;
    value = ((const volatile struct SystemAudioConfiguration *)0x1ff81000)->audio_value;
    data_00242d90.shared_status = value;
    func_001821f4(&bss_004b5234, value);
    func_00195ea4(&bss_004b6888);
    func_00146070(&bss_004b5234);
    func_0014837c(func_001861e0());
    func_00145f74(func_0013e84c, func_0013e9d8, func_0013e810);
    data_00242d90.published = 0;
    data_00242d90.pending = 0;
    data_00242d90.stopped = 0;
    data_00242d90.ready = 1;
    data_00242d90.initialized = 1;
    return 0;
}
