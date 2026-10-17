/* [INFERENCE] Recovered partial menu-scene layout; names describe observed roles. */
struct MenuScenePanel;
struct MenuChannelA;
struct MenuChannelB;
struct MenuSceneBase {
    const void *vtable;
    unsigned char reserved04[0x10];
    MenuScenePanel *panel;
    unsigned char reserved18[0x1c];
    int next_scene;
    MenuChannelA *channel_a;
    MenuChannelB *channel_b;
    unsigned char reserved40[0x50];
};

/* INTEGRATOR: move to include/game/menu_scene.h */
extern "C" {
int func_00177124(int channel);
void func_00177084(void);
void func_00181b58(int scene);
void func_00176e24(MenuSceneBase *scene);
void func_00176df4(void);
void func_00176dc8(int scene);
void func_00174480(void *object);
extern const unsigned data_0022359c[];
void func_001ab540(MenuSceneBase *scene);
}

void func_001ab540(MenuSceneBase *scene)
{
    scene->vtable = data_0022359c;
    if (!func_00177124(7)) {
        func_00177084();
        func_00181b58(3);
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->next_scene);
    }
    func_00174480(scene);
}
