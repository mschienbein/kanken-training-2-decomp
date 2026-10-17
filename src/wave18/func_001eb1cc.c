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

void func_001eb1cc(struct NotificationController *controller, unsigned index)
{
    float scales[3] = { 0.5f, 1.0f, 1.5f };
    controller->scale = scales[index];
}
