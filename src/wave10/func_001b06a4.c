#include "game/functions.h"
#include "game/selection.h"
#include "game/notification.h"
#include "game/animation.h"
#include "game/screen_transition.h"
extern int func_00172994(int channel);
extern int func_0017abf8(int channel, int sound);
extern void func_0017433c(int channel, int duration);






void func_001b06a4(struct SettingsScreen *screen)
{
    int index;
    for (index = 5; index < 16; ++index) {
        int value = func_00178f84(screen->controls, index);
        if (value != -1) {
            switch (index) {
            case 5:
                func_001dc3a4(value == 1);
                return;
            case 6: {
                int mode;
                func_0016fccc(value);
                mode = func_001758d4();
                if (mode == 3) {
                    func_0017433c(0, (int)func_0017adb4() * 60);
                } else if (func_00172994(0) != mode + 0x1000003) {
                    switch (func_001758d4()) {
                    case 0: func_0017abf8(0, 0x1000003); break;
                    case 1: func_0017abf8(0, 0x1000004); break;
                    case 2: func_0017abf8(0, 0x1000005); break;
                    }
                }
                return;
            }
            case 7:
                func_0016fca0(value);
                return;
            case 8:
                screen->next_state = 14;
                func_0016fc1c(screen, 12, 1, 1);
                return;
            case 9:
                func_0016fa58(screen, 1);
                return;
            case 10:
                func_001dc084(value == 0);
                return;
            case 11:
                func_001dc064(value == 0);
                return;
            case 12:
                func_0016fa38(value == 0);
                return;
            case 13:
                func_0016fa18(value == 0);
                return;
            case 14:
                func_0016fa58(screen, 0);
                return;
            case 15:
                switch (value) {
                case 0:
                    screen->next_state = 6;
                    func_0016fc1c(screen, 12, 1, 1);
                    break;
                case 1:
                    func_00172d48(-1);
                    screen->next_state = 4;
                    func_0016fc1c(screen, 12, 1, 1);
                    break;
                }
                return;
            }
            return;
        }
    }
}
