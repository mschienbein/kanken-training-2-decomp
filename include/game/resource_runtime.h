#ifndef GAME_RESOURCE_RUNTIME_H
#define GAME_RESOURCE_RUNTIME_H
#include "game/menu_scene.h"
#include "game/runtime.h"
#include "game/screen.h"

/* [INFERENCE] Resource-event names; array stride and active/owned-pointer fields
 * are established by cleanup and registry consumers, not zero stores alone. */
struct ResourceNode;
struct ResourceEventPool {
    unsigned reserved00[3];
    struct ResourceEvent *events;
    struct ResourceNode *first;
    struct ResourceNode *second;
    struct ResourceNode *third;
    int count;
    int active_count;
};
struct ResourceControlState {
    unsigned ready;
    unsigned state04;
    unsigned state08;
    unsigned char state0c;
    unsigned char reserved0d[3];
    int value;
};

struct AssetSlot { struct ResourceEvent *asset; };
struct DisplayResource4c {
    struct ResourceEvent *object;
    unsigned char reserved04[0x48];
};
struct DisplayResource50 {
    struct ResourceEvent *object;
    unsigned char reserved04[0x2c];
    void *allocation;
    unsigned char reserved34[0x1c];
};
struct DisplayResource6c {
    struct ResourceEvent *object;
    unsigned char reserved04[0x64];
    int texture_slot;
};
/* Common scene storage and native dispatch live in MenuScene. */
struct GeometryAssetSlot;
#ifdef __cplusplus
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern struct ResourceEventPool data_00242238;
extern struct ResourceControlState bss_00495c34, bss_00495c48, bss_00495c5c;
extern void func_0019d3b4(struct ResourceEvent *event);
extern void func_00183208(struct ResourceEvent *event);
extern void func_0018663c(struct ResourceEvent *event);
extern void func_00176e24(struct MenuScene *manager);
extern void func_001e3548(struct DeferredRequest *component);
void func_001c3c50(struct GeometryAssetSlot **slots);
extern void func_001b3434(struct MenuOptionGroup *component);
extern void func_001ea87c(struct VisualLabelSet72 *component);
extern void func_001e9cdc(struct MenuControlSet *component);
extern void func_001eaed8(struct StyleOwner *component);
extern void func_00168454(struct SelectionList24 *component);
extern void func_001af424(struct W6Selector *component);
extern void func_001acff0(struct W6HandlePanel10 *component);
extern void func_001682e8(struct SelectionList29 *component);
extern void func_001b6a0c(struct ScalarCollection *component);
extern void func_001b45ac(struct R41169OptionalPanel *component);
extern void func_001b41b8(struct AnimationState *component);
extern void func_001aeb30(struct RecordSelection4 *component);
extern void func_001acd38(struct SelectionSet2 *component);
extern void func_001ad708(struct MessageLayout *component);
extern void func_001b5e44(struct CharacterPanel *component);
extern void func_001b494c(struct L041HandwritingPanel *component);
extern void func_001b7604(struct Screen *component);
extern void func_001ae140(struct WidgetActionState *component);
extern void func_001b64b0(struct PairCollection *component);
extern void func_001dcf4c(struct BufferedSceneState *component);
extern void func_001dcdd0(struct IndexedSceneState *component);
extern void func_001b3564(struct MenuOptionGroup *component);
extern void func_001e9e7c(struct MenuControlSet *component);
extern void func_001b4310(struct AnimationState *component);
extern void func_001ad7a0(struct MessageLayout *component);
extern void func_001b6098(struct CharacterPanel *component);
extern void func_001b4d58(struct L041HandwritingPanel *component);
extern int func_001b6604(struct PairCollection *collection);
extern void func_001ebb14(struct MenuScene *scene, struct DisplayResource4c *entry);
extern void func_001eb95c(struct MenuScene *scene, struct DisplayResource50 *entry);
extern void func_001ebca0(struct MenuScene *scene, struct DisplayResource6c *entry);
void finish_scene_buffer(struct BufferedSceneState *component);
void finish_scene_indexed(struct IndexedSceneState *component);
#ifdef __cplusplus
}
#endif

#endif
