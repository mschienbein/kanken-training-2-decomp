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

extern struct NotificationController *func_001819f4(void);
extern void func_001eb1cc(struct NotificationController *controller, unsigned index);

void func_0016fca0(unsigned char mode)
{
    struct NotificationController *controller;
    bss_00280af0.payload.storage.fields.mode = mode;
    controller = func_001819f4();
    if (controller)
        func_001eb1cc(controller, bss_00280af0.payload.storage.fields.mode);
}
