#ifndef GAME_MENU_SCENE_H
#define GAME_MENU_SCENE_H

struct DisplayObject;
struct DisplayResource4c;
struct DisplayResource50;
struct DisplayResource6c;
struct W10DrawingInput;
struct BufferedSceneState;
struct IndexedSceneState;
struct DeferredRequest;
struct MenuOptionGroup;
struct VisualLabelSet72;
struct MenuControlSet;
struct StyleOwner;
struct SelectionList24;
struct W6Selector;
struct W6HandlePanel10;
struct SelectionList29;
struct ScalarCollection;
struct R41169OptionalPanel;
struct AnimationState;
struct RecordSelection4;
struct SelectionSet2;
struct MessageLayout;
struct CharacterPanel;
struct L041HandwritingPanel;
struct Screen;
struct WidgetActionState;
struct PairCollection;
struct MenuScene;

#ifdef __cplusplus
/* [INFERENCE] Native names. The constructor at 00174484 initializes this
 * 0x90-byte common scene; its singleton stores and the concrete constructors
 * establish the hierarchy, not coincident offsets. The ABI destructor occupies
 * slots 0/1; null derived entries are consistent with armlink virtual-function
 * elimination [INFERENCE], not additional callable methods.
 * Root table 002238cc has __cxa_pure_virtual in slots 2..7. Slot 7 has no
 * surviving body or caller: its neutral name and void signature are [INFERENCE],
 * carry no recovered callable contract, and must never be called. */
struct MenuScene {
    MenuScene(int first, int second, int third);
    virtual ~MenuScene();
    virtual void build() = 0;
    virtual void activate() = 0;
    virtual void update() = 0;
    virtual void depart() = 0;
    virtual void prepare() = 0;
    virtual void reserved_slot7() = 0;
    virtual void finish();

    unsigned reserved04;
    DisplayResource4c *first;
    DisplayResource50 *second;
    DisplayResource6c *third;
    W10DrawingInput *drawing;
    int state18, state1c;
    int first_count, second_count, third_count;
    unsigned state2c;
    unsigned char flag30, reserved31[3];
    int index;
    BufferedSceneState *buffered;
    IndexedSceneState *indexed;
    DeferredRequest *request;
    MenuOptionGroup *options;
    VisualLabelSet72 *labels;
    MenuControlSet *controls;
    StyleOwner *style;
    SelectionList24 *primary_list;
    W6Selector *selector;
    W6HandlePanel10 *handle_panel;
    SelectionList29 *navigation;
    ScalarCollection *scalar;
    R41169OptionalPanel *controller68;
    AnimationState *animation;
    RecordSelection4 *record_selection;
    SelectionSet2 *pair_selection;
    MessageLayout *message;
    CharacterPanel *characters;
    L041HandwritingPanel *controller80;
    Screen *indicators;
    WidgetActionState *controller88;
    PairCollection *transition;
};

extern "C" {
#endif
void func_00176e24(struct MenuScene *scene);
#ifdef __cplusplus
}
#endif

#endif
