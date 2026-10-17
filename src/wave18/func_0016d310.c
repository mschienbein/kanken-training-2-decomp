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
extern void func_00183208(struct ScreenAsset *asset);
extern void func_00180314(int index);
extern unsigned func_0014c338(void);

void func_0016d310(struct NotificationController *controller)
{
    int i;
    for (i = 0; i < 3; ++i) {
        if (controller->assets[i]) {
            func_00183208(controller->assets[i]);
            controller->assets[i] = 0;
        }
    }
    for (i = 0; i < 2; ++i) {
        if (controller->identifiers[i] != -1) {
            func_00180314(controller->identifiers[i]);
            controller->identifiers[i] = -1;
        }
    }
    if (controller->active == 1) {
        controller->previous = func_0014c338();
        controller->active = 0;
    }
}
