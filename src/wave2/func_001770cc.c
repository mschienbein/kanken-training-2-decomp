struct Animation {
    unsigned flags;
    unsigned char reserved04[0x64];
    float elapsed;
    float previous;
    unsigned char reserved70[0x0c];
    int frame;
};

struct AnimationOwner {
    unsigned char reserved00[0x20];
    struct Animation *animations[1];
};

void func_001770cc(struct AnimationOwner *owner, int index, int enabled)
{
    struct Animation *animation = owner->animations[index];
    if (animation != 0) {
        if (enabled) {
            animation->flags |= 2;
            animation->frame = -1;
        } else {
            animation->flags &= ~2u;
        }
        animation->previous = 0.0f;
        animation->elapsed = 0.0f;
    }
}
