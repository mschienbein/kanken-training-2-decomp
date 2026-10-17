#include "game/sound_runtime.h"
static __forceinline void initialize_type(SoundPlaybackState *object)
{
    object->vtable = data_0022459c;
}


extern "C" SoundPlaybackState *func_00180444(SoundPlaybackState *object)
{
    initialize_type(object);
    object->volume = 1.0f;
    object->pitch = 1.0f;
    object->values38[0] = 0.0f;
    object->values38[1] = 0.0f;
    object->values38[2] = 0.0f;
    object->values38[3] = 0.0f;
    object->values38[4] = 0.0f;
    object->fields4c[0] = 0;
    object->fields4c[1] = 0;
    object->fields4c[2] = 0;
    object->values58[0] = 1.0f;
    object->values58[1] = 1.0f;
    object->values60[0] = 0.0f;
    object->values60[1] = 0.0f;
    object->values60[2] = 0.0f;
    object->values60[3] = 0.0f;
    object->fields70[0] = 0;
    object->fields70[1] = 0;
    object->values78[0] = 0.0f;
    object->values78[1] = 0.0f;
    object->fields80[0] = 0;
    object->fields80[1] = 0;
    object->values88[0] = 0.0f;
    object->values88[1] = 0.0f;
    object->fields90[0] = 0;
    object->fields90[1] = 0;
    object->active = false;
    object->valuesb8[0] = 0.0f;
    object->valuesb8[1] = 0.0f;
    object->fieldsc0[0] = 0;
    object->fieldsc0[1] = 0;
    object->field_ec = 0;
    object->field_f0 = 0;
    object->field_f4 = 0;
    object->runtime_link.next = 0;
    object->runtime_link.previous = 0;
    object->fields100[0] = 0;
    object->fields100[1] = 0;
    object->fields100[2] = 0;
    object->fields100[3] = 0;
    object->fields100[4] = 0;
    object->fields100[5] = 0;
    return object;
}
