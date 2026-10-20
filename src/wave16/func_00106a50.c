#include "game/functions.h"
#include "game/sound_task.h"
#include "game/object_manager.h"
#include "game/service_state.h"
#include "game/applet.h"













void func_00106a50(void)
{
    data_0023fdd8.reasons = 0;
    func_0010ce58(func_0010b610, 0);
    func_0010ce44(func_0010b5cc, 0);
    func_0010cf84(func_0010b5f0, 0);
    func_0019df28(&data_0023fdd8.event, 1);
    func_00195ec4(&bss_0029f528);
    func_00195ec4(&bss_0029f534);
    func_0019ea6c(&data_0023fdd8.event);
    func_0010d454(0);
    if (func_00195e94() != 0 || func_00195e84() != 0)
        data_0023fdd8.ready = 1;
}
