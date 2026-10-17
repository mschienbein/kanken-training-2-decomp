#include "game/functions.h"

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

union ControllerGuard {
    unsigned word;
    unsigned char initialized;
};
extern volatile union ControllerGuard data_0023ee98;
extern struct NotificationController bss_0028c860;
extern struct NotificationController *func_0014992c(struct NotificationController *controller);
extern struct NotificationController *func_00149a98(struct NotificationController *controller);

struct NotificationController *func_001819f4(void)
{
    if (!(data_0023ee98.initialized & 1) && func_00191c80(&data_0023ee98.word)) {
        struct NotificationController *controller = func_0014992c(&bss_0028c860);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00216140(controller, (ObjectDestructor)func_00149a98, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_0023ee98.word);
    }
    return &bss_0028c860;
}
