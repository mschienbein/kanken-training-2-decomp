#include "game/screen.h"

struct AnimationBank;
struct MotionRange;
struct MotionTarget;

struct AnimationScene {
    unsigned char reserved00[0x44];
    struct AnimationBank *bank;
    unsigned char reserved48[0x24];
    struct MotionRange *progress;
    unsigned char reserved70[8];
    struct MotionTarget *target;
    unsigned char reserved7c[8];
    struct Screen *screen;
    unsigned char reserved88[8];
    int selected;
    int previous;
    int substates[8];
    int timer;
    unsigned char reservedbc[0x48];
    int result;
};

extern void func_00160694(struct AnimationScene *scene);
extern void func_00161240(struct AnimationScene *scene);
extern void func_001608b4(struct AnimationScene *scene);

void func_0015f8bc(struct AnimationScene *scene)
{
    switch (scene->selected) {
    case 0:
        func_00161240(scene);
        break;
    case 1:
        func_001608b4(scene);
        break;
    case 2:
        func_00160694(scene);
        break;
    }
}
