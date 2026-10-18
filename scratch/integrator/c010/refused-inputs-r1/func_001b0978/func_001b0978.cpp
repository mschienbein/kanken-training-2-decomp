#include <stdint.h>
#include "game/deferred_callback.h"
#include "game/notification.h"
#include "game/runtime.h"
#include "game/selection.h"
#include "game/w11_control.h"
#include "game/input.h"

struct L006Widget;
struct L006BookView;
struct L006CategoryView;
struct L006AnimationKeys {
    uint32_t fields[18];
};
struct W11jCalendar {
    unsigned char reserved00[0x0c];
    int year;
    int month;
    int day;
};
struct TransitionController {
    unsigned char reserved00[0x34];
    unsigned char active;
    unsigned char paused;
    unsigned char reserved36[2];
    float elapsed;
    unsigned char reserved3c[4];
    int mode;
};
struct L006InitScene {
    unsigned char reserved00[0x44];
    L006Widget *text;
    W11jVisualOwner *icon;
    W6ListController *menu;
    W8DisplaySet *portrait;
    SelectionList *toolbar;
    unsigned reserved58;
    L006Widget *empty_panel_a;
    AlternateSelectionList *header;
    L006Widget *footer;
    L006Widget *empty_panel_b;
    unsigned reserved6c[2];
    W11jWidgetGroup *subpanel;
    unsigned char reserved78[0x14];
    W11jWidgetGroup *modal;
    int state;
    float elapsed;
    int index;
    int mode;
    int previous;
    int active;
    int selected;
    unsigned reservedac[2];
    unsigned char enabled;
    unsigned char visible;
    unsigned char reservedb6[2];
    int year;
    int month;
    int current_year;
    int current_month;
    unsigned char reservedc8[0x158];
    L006BookView *book_view;
    W11jCalendar *date_view;
    L006CategoryView *category_view;
    int entry;
    int16_t ranges[4];
    float progress;
};

/* INTEGRATOR: move to include/game/calendar_scene.h */
extern "C" {
TransformEntry *func_00174454(int group, int index);
int func_00175838(void);
void func_001c1b14(L006BookView *view);
void func_001c2a34(W11jCalendar *view);
void func_001c2dd0(L006CategoryView *view);
void func_0015dc70(int *year, int *month, int *day, int *hour,
                   int *minute, int *second, int *weekday, int *yearday);
void func_001dc580(int index, int *year, int *month, int *day);
void func_001c2280(W11jCalendar *view, int year, int month, int day);
void func_00170f20(int index);
void func_00172b38(L006Widget *widget);
void func_00174480(void *object);
void func_00175d90(W11jVisualOwner *owner);
void func_0016f9a8(W11jVisualOwner *owner, int index, float x, float y);
void func_001eaa7c(W11jVisualOwner *owner, int index,
                   unsigned char red, unsigned char green,
                   unsigned char blue, unsigned char alpha);
void func_00175a5c(AnimationOwner *owner, int index,
                   float x, float y, float width, float height);
void func_00179e40(AnimationOwner *owner, int index,
                   float x, float y, float width, float height);
void func_0016f930(const InputOwner *owner, int index);
void func_001758e4(L006Widget *widget);
void func_001729e4(W11jWidgetGroup *group);
void func_00172c04(TransitionController *controller, int mode);
void func_00172c28(W11jWidgetGroup *group);
void func_00173f9c(TransitionController *controller, int paused);
void func_001729bc(W11jWidgetGroup *group);
void func_001e96d4(W6ListController *control, int index,
                   const L006AnimationKeys *keys);
extern const L006AnimationKeys data_00218508;
extern const MemberCallback data_002184d8;
void func_001b0978(L006InitScene *scene);
}

void func_001b0978(struct L006InitScene *scene)
{
    if (func_0019efd0() != 0)
        return;
    func_00176d98();
    scene->state = 0;
    if (func_00174454(4, 0)->count == 99)
        scene->mode = 4;
    else
        scene->mode = 2;
    scene->previous = -1;
    scene->active = 1;
    scene->elapsed = 0.0f;
    scene->index = -1;
    scene->selected = -1;
    if (func_00175838() == 10)
        scene->mode = 7;
    scene->enabled = 1;
    scene->visible = 1;
    scene->entry = -1;
    scene->ranges[0] = -1;
    scene->ranges[1] = -1;
    scene->ranges[2] = -1;
    scene->ranges[3] = -1;
    scene->progress = 0.0f;
    func_001c1b14(scene->book_view);
    func_001c2a34(scene->date_view);
    func_001c2dd0(scene->category_view);
    {
        int year = 2014, month = 9, day = 26;
        func_0015dc70(&year, &month, &day, 0, 0, 0, 0, 0);
        scene->year = year;
        scene->month = month;
        func_001dc580(-1, &scene->current_year, &scene->current_month, &day);
        func_001c2280(scene->date_view, scene->current_year,
                      scene->current_month, day);
    }
    func_00170f20(7);
    func_00170f20(8);
    func_00170f20(9);
    func_00170f20(13);
    if (scene->toolbar) func_00176b24(scene->toolbar);
    /* [INFERENCE] callee identity: inlined by armlink. */
    if (scene->empty_panel_a) func_00174480(scene->empty_panel_a);
    if (scene->header) func_0017697c(scene->header);
    if (scene->footer) func_00172b38(scene->footer);
    /* [INFERENCE] callee identity: inlined by armlink. */
    if (scene->empty_panel_b) func_00174480(scene->empty_panel_b);
    if (scene->icon) {
        func_00175d90(scene->icon);
        func_0016f9a8(scene->icon, 0, 0.75f, 0.75f);
        func_001eaa7c(scene->icon, 0, 170, 105, 65, 255);
        func_00173fb4(reinterpret_cast<Screen *>(scene->icon));
    }
    if (scene->portrait) {
        func_00175b8c(scene->portrait);
        func_00179160(scene->portrait, 0, 0.8f, 1.0f);
        func_00175ab8(scene->portrait, 0, 79, 79, 79, 255);
    }
    if (scene->menu) {
        func_001e9944(scene->menu);
        func_00175a5c(reinterpret_cast<AnimationOwner *>(scene->menu),
                       3, 0.0f, 0.0f, 320.0f, 194.0f);
        func_00179e40(reinterpret_cast<AnimationOwner *>(scene->menu),
                       3, 0.0f, 0.0f, 576.0f, 0.0f);
        func_00179068(reinterpret_cast<W7dMenuControl *>(scene->menu), 3, 1);
        func_0017365c(scene->menu, 3, 4, 1, -1, 0);
        func_00179600(scene->menu, 15, 0);
        func_0016f930(reinterpret_cast<const InputOwner *>(scene->menu), 5);
        func_0016f930(reinterpret_cast<const InputOwner *>(scene->menu), 6);
        func_0016f930(reinterpret_cast<const InputOwner *>(scene->menu), 7);
        func_0016f930(reinterpret_cast<const InputOwner *>(scene->menu), 10);
        func_0016f930(reinterpret_cast<const InputOwner *>(scene->menu), 11);
        func_0016f930(reinterpret_cast<const InputOwner *>(scene->menu), 12);
        func_0016f930(reinterpret_cast<const InputOwner *>(scene->menu), 13);
    }
    if (scene->text) func_001758e4(scene->text);
    if (scene->modal) {
        func_001729e4(scene->modal);
        func_00172c04(reinterpret_cast<TransitionController *>(scene->modal), 0);
        func_00172c28(scene->modal);
        func_00173f9c(reinterpret_cast<TransitionController *>(scene->modal), 1);
    }
    if (scene->subpanel) func_001729bc(scene->subpanel);
    if (func_00174454(4, 0)->count != 99) {
        struct L006AnimationKeys keys;
        func_00192720(&keys, &data_00218508, sizeof(keys));
        func_001e96d4(scene->menu, 3, &keys);
    }
    /* [INFERENCE] The scene is the callback receiver. The original named
     * descriptor retains the member-function identity and adjustment. */
    func_00177b10(reinterpret_cast<CallbackTarget *>(scene), data_002184d8);
    func_00175848();
}
