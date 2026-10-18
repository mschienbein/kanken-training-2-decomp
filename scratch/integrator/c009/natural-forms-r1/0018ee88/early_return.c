#include "game/applet.h"
#include "game/service_callbacks.h"
#include "game/service_state.h"

void func_0018ee88(unsigned char notify)
{
    data_00242a6c.status = 0;
    if (!notify)
        return;
    unsigned char mode = 0;
    func_00192400(3);
    func_001923d4(mode);
}
