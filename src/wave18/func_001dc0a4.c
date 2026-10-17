#include "game/notification.h"

struct ScreenAsset;
struct NotificationController {
    unsigned char reserved00[4];
    int identifiers[2];
    struct ScreenAsset *assets[3];
    unsigned char active;
    unsigned char reserved19[0x881];
    unsigned short previous;
    unsigned char reserved89c[0x20];
    float scale;
};

extern void func_00192720(void *destination, const void *source, unsigned size);
extern struct NotificationController *func_001819f4(void);
extern void func_001eb1cc(struct NotificationController *controller, unsigned index);
extern unsigned func_001699b8(void);

void func_001dc0a4(int index)
{
    struct NotificationController *controller;
    bss_00280af0.index = index;
    func_00192720(&bss_00280af0.payload, &bss_002476b0[index],
                  sizeof(struct NotificationPayload));
    controller = func_001819f4();
    if (controller)
        func_001eb1cc(controller, bss_00280af0.payload.storage.fields.mode);
    bss_0028baf0.generation = func_001699b8();
}
