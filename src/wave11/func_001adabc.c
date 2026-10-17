struct WidgetActionState {
    unsigned char reserved00[0xbc];
    unsigned active;
};

void func_001adabc(struct WidgetActionState *state)
{
    state->active = 0;
}
