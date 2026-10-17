/* INTEGRATOR: differs from include/game/selection.h: the refresh body visits twenty list slots and this wrapper clears the byte at 0x74. */
struct W6ListGroup;
struct W6ListController {
    unsigned reserved00[8];
    struct W6ListGroup *lists[20];
    unsigned char reserved70[4];
    unsigned char refreshing;
};

extern void func_001e9950(struct W6ListController *controller);

void func_001e9944(struct W6ListController *controller)
{
    controller->refreshing = 0;
    func_001e9950(controller);
}
