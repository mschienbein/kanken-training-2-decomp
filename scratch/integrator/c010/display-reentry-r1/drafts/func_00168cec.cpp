#include "game/screen.h"

/* The original forwards entry r1 unchanged as the full-word alpha argument.
 * The dispatcher reads this same WidgetController slot vector at +0x0c. */
void func_00168cec(WidgetController *controller, int alpha)
{
    func_0016e300(controller, 6, 255, 255, 255, alpha);
}
