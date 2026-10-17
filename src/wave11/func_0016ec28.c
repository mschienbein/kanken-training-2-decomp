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

extern void func_0016e924(struct KeyAnimation *animation, float delta);

void func_0016ec28(struct KeyAnimation *animation, int frame)
{
    animation->mode = 2;
    if (frame != -1) {
        animation->current = frame;
        animation->time = animation->keys[frame].time;
        animation->first = frame;
        animation->first_time = animation->keys[frame].time;
        func_0016e924(animation, 0.0f);
    }
}
