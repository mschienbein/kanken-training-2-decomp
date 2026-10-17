#ifndef GAME_SCENE_DISPATCH_H
#define GAME_SCENE_DISPATCH_H

struct SceneDispatch;
struct SceneObject;



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */


struct SceneDispatch {
    unsigned char reserved00[8];
    void (*slot02)(SceneObject *object);
    unsigned char reserved0c[8];
    void (*slot05)(SceneObject *object);
};
struct SceneObject {
    const SceneDispatch *vtable;

    void invoke_slot02()
    {
        vtable->slot02(this);
    }

    void invoke_slot05()
    {
        vtable->slot05(this);
    }
};

extern "C" {

extern SceneObject bss_0028e848;
extern SceneObject bss_0028e1e0;
extern SceneObject bss_0028f080;
extern SceneObject bss_0028dfa4;
extern SceneObject bss_0028ecf4;
extern SceneObject bss_0028e318;
extern SceneObject bss_0028e790;
extern SceneObject bss_0028e404;
extern SceneObject bss_0028e27c;
extern SceneObject bss_0028f3d8;
extern SceneObject bss_0028db2c;
extern SceneObject bss_0028f850;
extern SceneObject bss_0028fd24;
extern SceneObject bss_0028dd7c;
}
#endif

#endif
