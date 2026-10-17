struct SceneTransitionState {
    unsigned char reserved00[0x34];
    int next_scene;
};
struct SceneTransitionSelection {
    struct SceneTransitionState common;
    unsigned char reserved38[0x58];
    int next_scene;
};

extern int func_00177124(unsigned mask);
extern void func_00177084(void);
extern void func_00176e24(struct SceneTransitionState *state);
extern void func_00176df4(void);
extern void func_00176dc8(int scene);

void func_001e7564(struct SceneTransitionSelection *state)
{
    if (!func_00177124(7)) {
        func_00177084();
        func_00176e24(&state->common);
        func_00176df4();
        func_00176dc8(state->next_scene);
    }
}
