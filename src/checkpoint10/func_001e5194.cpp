#include "game/playback_screen.h"
#include "game/screen.h"
#include "game/runtime.h"

struct FadeInputState;
class FadeController {
public:
    virtual ~FadeController();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void update();
    virtual void slot07();
    virtual void finish();
    unsigned char reserved04[0x10];
    FadeInputState *input;
    unsigned char reserved18[0x168];
    DisplayObject *target;
    unsigned char reserved184[0x20];
    MotionState motion;
};

/* INTEGRATOR: move to include/game/controller.h */
extern "C" {
void func_0016d038(FadeInputState *input);

void func_00170088(DisplayData *object, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
void func_00170030(BoundDisplayObject *object, unsigned red, unsigned green, unsigned blue, unsigned alpha);
void func_00170014(DisplayData *object, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
void func_0016fff8(DisplayData *object, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
}

static inline void set_colour(DisplayObject *object, unsigned char red, unsigned char green,
                              unsigned char blue, unsigned char alpha)
{
    if (object) {
        int type = func_0017a9a0(object, 0);
        if (type == 3) {
            func_00170088(static_cast<DisplayData *>(static_cast<void *>(object)), red, green, blue, alpha);
        } else if (type != 5) {
            if (type == 6)
                func_00170030(static_cast<BoundDisplayObject *>(static_cast<void *>(object)), red, green, blue, alpha);
        } else {
            func_00170014(static_cast<DisplayData *>(static_cast<void *>(object)), red, green, blue, alpha);
            func_0016fff8(static_cast<DisplayData *>(static_cast<void *>(object)), red, green, blue, alpha);
        }
    }
}

extern "C" void func_001e5194(FadeController *controller)
{
    controller->update();
    func_0016d038(controller->input);
    set_colour(controller->target, 79, 79, 79,
               (unsigned char)(unsigned)func_0016e874(&controller->motion));
    func_0016e924(&controller->motion, -1.0f);
    controller->finish();
}
