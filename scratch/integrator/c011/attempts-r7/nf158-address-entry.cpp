#include "game/input_device.h"

/* [INFERENCE] This typed coordinate initializer's historical identity is unknown. */
static void reset_coordinates(InputCoordinates *coordinates, float value)
{
    coordinates->first = value;
    coordinates->second = value;
    coordinates->third = value;
    coordinates->fourth = value;
}

/* [INFERENCE] Reset/helper identities and common-TU membership are unrecovered. */
static void reset_input_state(InputDeviceState *state, float coordinate_value)
{
    state->flags[0] = 0;
    state->flags[1] = 0;
    state->flags[2] = 0;
    state->flags[3] = 0;
    reset_coordinates(&state->coordinates, coordinate_value);
    state->counts[0] = 0;
    state->counts[1] = 0;
    state->counts[2] = 0;
    state->counts[3] = 0;
    state->counts[4] = 0;
    state->counts[5] = 0;
    InputStoragePointers pointers = {
        state->first_storage, state->second_storage, state->third_storage, state->fourth_storage
    };
    state->storage = pointers;
    state->ready = 0;
}

extern "C" InputDeviceState *func_00116eb0(InputDeviceState *object)
{
    object->vtable = &data_00224bf8;
    func_00123b38(&object->first);
    func_00123b38(&object->second);
    reset_input_state(object, 0.0f);
    return object;
}
