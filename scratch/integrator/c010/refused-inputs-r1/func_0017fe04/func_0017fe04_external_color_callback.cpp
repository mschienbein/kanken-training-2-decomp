#include <preinclude.h>
#include <nw/lyt/lyt_Material.h>
#include "game/screen.h"

/* INTEGRATOR: the frozen Color8 inline constructor emits a 16-byte callback
 * under middleware, unlike this image's retained 12-byte func_0014b438. Keep
 * this callback external; promote the image-specific constructor declarations
 * and reconcile with src/wave10/func_0014b438.c before collection.
 * [INFERENCE] Descriptive class names; callback role follows the natural
 * seven-element color-array caller, not a guessed pure call-target binding. */
class MaterialColor {
    unsigned packed;
public:
    MaterialColor();
};
class MaterialInit {
public:
    MaterialInit();
    virtual ~MaterialInit();
private:
    nw::lyt::AnimationList animations;
    MaterialColor colors[7];
    nw::lyt::internal::MatMemNums capacity;
    nw::lyt::internal::MatMemNums count;
    void *memory;
    char name[21];
    unsigned char flags;
    unsigned char padding[2];
    void Init();
};
MaterialInit::MaterialInit()
{
    Init();
    func_0019cecc(name, sizeof(name));
}
