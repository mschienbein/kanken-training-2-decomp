struct W19ScrollState {
    int maximum;
    int value;
};
struct W19ScrollOwner {
    unsigned char reserved00[0x38];
    struct W19ScrollState scroll;
    int lower;
    int length;
};
extern void func_0016a380(struct W19ScrollState *scroll, int amount, int origin);

void func_001d34c0(struct W19ScrollOwner *owner, int value, int mode)
{
    switch (mode) {
    case 0:
        value += owner->lower;
        break;
    case 1:
        value += owner->scroll.value;
        break;
    case 2:
        value = owner->lower + owner->length - value;
        break;
    default:
        return;
    }
    if (value < owner->lower)
        value = owner->lower;
    else if (value > owner->lower + owner->length)
        value = owner->lower + owner->length;
    func_0016a380(&owner->scroll, value, 0);
}
