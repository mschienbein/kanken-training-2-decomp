struct BaseState {
    unsigned char reserved00[0x0c];
    unsigned char flag0c;
    unsigned char flag0d;
    unsigned char flag0e;
    unsigned char flag0f;
    unsigned char flag10;
    float scale_x;
    float scale_y;
    float value1c;
    float value20;
    float value24;
    float value28;
    unsigned char level;
    unsigned char reserved2d;
    unsigned char flag2e;
    float value30;
    unsigned char flag34;
    unsigned char flag35;
    float value38;
    float value3c;
    unsigned int value40;
};

void func_0016a068(struct BaseState *state)
{
    state->flag0c = 0;
    state->flag0d = 0;
    state->flag0e = 0;
    state->flag0f = 0;
    state->flag10 = 0;
    state->scale_x = 1.0f;
    state->scale_y = 1.0f;
    state->value1c = 0.0f;
    state->value20 = 0.0f;
    state->value24 = 0.0f;
    state->flag2e = 0;
    state->level = 255;
    state->value28 = 0.0f;
    state->value30 = 0.0f;
    state->flag34 = 0;
    state->flag35 = 0;
    state->value38 = 0.0f;
    state->value3c = 0.0f;
    state->value40 = 0;
}
