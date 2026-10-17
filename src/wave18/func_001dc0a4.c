#include "game/functions.h"
#include "game/notification.h"


extern void func_00192720(void *destination, const void *source, unsigned size);



void func_001dc0a4(int index)
{
    struct W10DrawingInput *controller;
    bss_00280af0.index = index;
    func_00192720(&bss_00280af0.payload, &bss_002476b0[index],
                  sizeof(struct NotificationPayload));
    controller = func_001819f4();
    if (controller)
        func_001eb1cc(controller, bss_00280af0.payload.storage.fields.mode);
    bss_0028baf0.generation = func_001699b8();
}
