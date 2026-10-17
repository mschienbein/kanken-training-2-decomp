struct AnimationKey {
    float time;
    unsigned flags;
    unsigned char reserved08[0x10];
};

struct KeyAnimation {
    int mode;
    unsigned key_flags;
    unsigned flags;
    float time;
    float speed;
    float first_time;
    float last_time;
    int first;
    int last;
    int current;
    float blend;
    int count;
    struct AnimationKey *keys;
};

void func_0016ec6c(struct KeyAnimation *animation, int looping)
{
    unsigned flags = animation->flags;
    animation->flags = looping ? flags & ~0x10u : flags | 0x10u;
}
