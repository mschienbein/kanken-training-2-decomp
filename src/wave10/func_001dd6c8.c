#include "game/selection.h"

extern int func_00178f84(struct W6ListController *controller, int index);
extern void func_0016fccc(int selection);
extern int func_001758d4(void);
extern int func_00172994(int index);
extern float func_0017adb4(void);
extern void func_0017433c(int index, int ticks);
extern int func_0017abf8(int index, int resource);
extern void func_0016fca0(int mode);
extern void func_0016fa38(int enabled);
extern void func_0016fa18(int enabled);

void func_001dd6c8(struct W6SelectionScreen *screen)
{
    int value = func_00178f84(screen->list, 4);
    if (value != -1) {
        int selection;
        switch (value) {
        case 0: func_0016fccc(0); break;
        case 1: func_0016fccc(1); break;
        case 2: func_0016fccc(2); break;
        case 3: func_0016fccc(3); break;
        }
        selection = func_001758d4();
        if (selection == 3) {
            func_0017433c(0, (int)func_0017adb4() * 60);
        } else if (func_00172994(0) != selection + 0x01000003) {
            switch (func_001758d4()) {
            case 0: func_0017abf8(0, 0x01000003); break;
            case 1: func_0017abf8(0, 0x01000004); break;
            case 2: func_0017abf8(0, 0x01000005); break;
            }
        }
    } else {
        value = func_00178f84(screen->list, 5);
        if (value != -1) {
            switch (value) {
            case 0: func_0016fca0(0); break;
            case 1: func_0016fca0(1); break;
            case 2: func_0016fca0(2); break;
            }
        } else {
            value = func_00178f84(screen->list, 6);
            if (value != -1) {
                switch (value) {
                case 0: func_0016fa38(0); break;
                case 1: func_0016fa38(1); break;
                }
            } else {
                value = func_00178f84(screen->list, 7);
                if (value != -1) {
                    switch (value) {
                    case 0: func_0016fa18(0); break;
                    case 1: func_0016fa18(1); break;
                    }
                }
            }
        }
    }
}
