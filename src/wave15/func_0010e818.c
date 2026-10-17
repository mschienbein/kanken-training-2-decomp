#include "game/functions.h"
#include "game/notification.h"

struct SavedNotificationInitialization {
    int index;
    unsigned reserved04;
    struct NotificationPayload payload;
    unsigned char reservedb748[0x18];
    int secondary_index;
    unsigned char reservedb764[0x60c];
};

/* The saved object also contains state beyond the published payload view. */
union SavedNotificationInitializationView {
    struct SavedNotification saved;
    struct SavedNotificationInitialization initialization;
};

extern void func_0019cecc(void *destination, unsigned int size);
extern void func_00190e44(struct NotificationPayload *payload);
extern unsigned char data_0023ee90;

void func_0010e818(void)
{
    union SavedNotificationInitializationView *view;
    int index;

    func_0019cecc(&bss_00280af0, sizeof(union SavedNotificationInitializationView));
    view = (union SavedNotificationInitializationView *)&bss_00280af0;
    view->initialization.index = -1;
    view->initialization.secondary_index = -1;
    func_00190e44(&view->initialization.payload);
    for (index = 0; index < 5; ++index)
        func_00190e44(&bss_002476b0[index]);
    data_0023ee90 = 1;
}
