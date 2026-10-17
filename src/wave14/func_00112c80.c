#include "game/service_list.h"
#include "game/service_protocol.h"
#include "game/service_result.h"
#include "game/service_state.h"


void func_00112c80(struct ServiceObject *object)
{
    struct ServiceContext *state = func_0018fcb4();
    func_00112c98(&state->reset, object->handle);
}
