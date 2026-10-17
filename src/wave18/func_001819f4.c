#include "game/functions.h"

#include "game/screen.h"

union ControllerGuard {
    unsigned word;
    unsigned char initialized;
};
extern volatile union ControllerGuard data_0023ee98;
extern struct W10DrawingInput bss_0028c860;
extern struct W10DrawingInput *func_0014992c(struct W10DrawingInput *controller);


struct W10DrawingInput *func_001819f4(void)
{
    if (!(data_0023ee98.initialized & 1) && func_00191c80(&data_0023ee98.word)) {
        struct W10DrawingInput *controller = func_0014992c(&bss_0028c860);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00216140(controller, (ObjectDestructor)func_00149a98, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_0023ee98.word);
    }
    return &bss_0028c860;
}
