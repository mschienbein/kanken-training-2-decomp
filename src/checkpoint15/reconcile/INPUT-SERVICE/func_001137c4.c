#include <string.h>
#include "ctr/svc.h"
#include "game/runtime.h"
#include "game/input.h"
#include "game/applet.h"
#include "game/service_availability.h"

static __forceinline void assert_success(CtrResult result)
{
    switch ((unsigned)result >> 31) {
    case 0: break;
    default: func_0019f568(); break;
    }
}

int func_001137c4(struct InputService *service, const char *service_name)
{
    unsigned char *initialized = &data_00242c38;
    struct InputService *state = service;
    unsigned memory = 0;
    unsigned buttons = 0;
    unsigned touch = 0;
    unsigned sensor = 0;
    unsigned motion = 0;
    unsigned auxiliary = 0;
    CtrResult result;
    struct InputSharedMemory *address;
    const volatile struct MotionRing *motion_view;
    const volatile struct TouchRing *touch_view;
    const volatile struct SensorRing *sensor_view;
    const volatile struct AuxiliaryInputRing *auxiliary_view;

    if (*initialized)
        return (CtrResult)0xe0a04ff9u;
    result = func_0019edcc();
    if (((unsigned)result & 0x3ff) != 0x3f9)
        assert_success(result);
    result = func_00192b44(&data_0024311c.handle, service_name, strlen(service_name), 0);
    assert_success(result);
    result = (CtrResult)func_00121d28(&memory, &buttons, &touch, &sensor, &motion, &auxiliary);
    assert_success(result);
    /* ORIGINAL BUG: the map result is ignored before bank pointers are published. */
    func_0013672c(&state->mapping, memory, 0x2b0, 1);
    address = state->mapping.address;
    touch_view = &address->touch;
    sensor_view = &address->sensor;
    motion_view = &address->motion;
    auxiliary_view = &address->auxiliary;
    state->ring = &address->buttons;
    state->motion.ring = motion_view;
    state->touch.ring = touch_view;
    state->sensor.ring = sensor_view;
    state->auxiliary.ring = auxiliary_view;
    state->handle = buttons;
    state->touch.handle = touch;
    state->sensor.handle = sensor;
    state->motion.handle = motion;
    state->auxiliary.handle = auxiliary;
    *initialized = 1;
    return 0;
}
