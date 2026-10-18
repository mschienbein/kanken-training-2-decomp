#include "game/runtime.h"
#include "game/command_state.h"
#include "game/deferred_callback.h"
#include "game/notification.h"
#include "game/item_entries.h"

void *operator new(unsigned size) throw();

struct MenuParameterObject {
    virtual ~MenuParameterObject();
};

/* This is the layout used by src/wave3/func_001748b8.c. */
struct ArrayOwner {
    unsigned char reserved00[8];
    void *elements;
};

struct MenuTextParameter : ParameterValue {
    unsigned flags;
    unsigned style;
    float x;
    float y;
    float z;
    const unsigned short *text;
};

struct MenuPlacementParameter : MenuParameterObject {
    unsigned short resource;
    unsigned short index;
    unsigned short state;
    unsigned short reserved0a;
    float x;
    float y;
    float z;
    unsigned flags;
    MenuPlacementParameter(unsigned short resource, unsigned short index,
                           float x, float y, float z, unsigned flags = 0);
};

struct MenuValueTarget;
struct MenuTextTarget;
struct MenuGroupTarget;
struct W8EffectState;

struct TrainingMenuState {
    unsigned char reserved00[0x14];
    W8EffectState *display;
    unsigned char reserved18[0x34];
    MenuGroupTarget *groups;
    MenuTextTarget *text;
    MenuValueTarget *single_values;
    MenuValueTarget *first_pairs;
    MenuValueTarget *second_pairs;
    MenuValueTarget *third_pairs;
    MenuValueTarget *last_values;
};

/* INTEGRATOR: move to include/game/menu_parameters.h */
extern "C" {
extern const MemberCallback data_00218680;
void func_0019eb48(void);
ParameterValue *func_001757d0(ParameterValue *parameter);
ParameterValue *func_00172988(ParameterValue *parameter);
ParameterValue *func_0017280c(ParameterValue *parameter);
ParameterValue *func_00175648(ParameterValue *parameter);
ParameterValue *func_001725e8(ParameterValue *parameter);
void func_00175654(MenuValueTarget *target, ParameterValue *const *parameters);
void func_00172818(MenuValueTarget *target, ParameterValue *const *parameters);
void func_001725f4(MenuValueTarget *target, ParameterValue *const *parameters);
void func_001754d0(MenuValueTarget *target, ParameterValue *const *parameters);
void func_0017245c(MenuValueTarget *target, ParameterValue *const *parameters);
/* INTEGRATOR: differs from src/wave11/func_00174f04.c: callers pass a UTF-16 text pointer in the stored extra word, not an integer. */
MenuTextParameter *func_00174f04(MenuTextParameter *parameter, const unsigned short *text,
                                unsigned flags, unsigned style,
                                float x, float y, float z);
ParameterValue *func_00174ef8(ParameterValue *parameter);
void func_00174df0(MenuTextTarget *target, MenuTextParameter *const *parameters);
MenuPlacementParameter *func_00174d9c(MenuPlacementParameter *parameter,
                                     unsigned short resource, unsigned short index,
                                     float x, float y, float z, unsigned flags);
ArrayOwner *func_00174cc8(ArrayOwner *owner, int index, int count, ...);
ArrayOwner *func_001748b8(ArrayOwner *owner);
void func_001748ec(MenuGroupTarget *target, ArrayOwner *const *groups);
W8EffectState *func_001819f4(void);
void func_00181994(W8EffectState *display);
void func_0018191c(W8EffectState *display);
void func_0016dc94(W8EffectState *display, int enabled);
void func_001e489c(TrainingMenuState *menu);
}

template <typename T, int Count>
static void ReleaseParameters(T *(&parameters)[Count])
{
    for (int i = 0; i < Count; ++i) {
        if (parameters[i]) {
            /* [INFERENCE] callee identity: empty parameter destructor inlined by armlink. */
            func_001852f4(func_001ef884(parameters[i]));
            parameters[i] = 0;
        }
    }
}

static void ReleaseGroups(ArrayOwner *(&parameters)[7])
{
    for (int i = 0; i < 7; ++i) {
        if (parameters[i]) {
            func_001852f4(func_001748b8(parameters[i]));
            parameters[i] = 0;
        }
    }
}

/* ORIGINAL BUG: failed record allocations are still forwarded as null list
 * entries. The list-copy routines dereference entries before testing their
 * sentinel fields; do not silently omit or replace failed entries here. */
void func_001e489c(TrainingMenuState *menu)
{
    func_0019eb48();
    if (menu->single_values) {
        ParameterValue *parameters[2];
        ParameterValue *value = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (value)
            value->value = 12;
        parameters[0] = value;
        ParameterValue *end = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (end)
            end = func_001757d0(end);
        parameters[1] = end;
        func_00175654(menu->single_values, parameters);
        ReleaseParameters(parameters);
    }
    if (menu->first_pairs) {
        ParameterValue *parameters[3];
        ParameterValue *value = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (value)
            value->value = 133;
        parameters[0] = value;
        value = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (value)
            value->value = 123;
        parameters[1] = value;
        ParameterValue *end = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (end)
            end = func_00172988(end);
        parameters[2] = end;
        func_00172818(menu->first_pairs, parameters);
        ReleaseParameters(parameters);
    }
    if (menu->second_pairs) {
        ParameterValue *parameters[3];
        ParameterValue *value = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (value)
            value->value = 34;
        parameters[0] = value;
        value = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (value)
            value->value = 40;
        parameters[1] = value;
        ParameterValue *end = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (end)
            end = func_0017280c(end);
        parameters[2] = end;
        func_001725f4(menu->second_pairs, parameters);
        ReleaseParameters(parameters);
    }
    if (menu->third_pairs) {
        ParameterValue *parameters[3];
        ParameterValue *value = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (value)
            value->value = 17;
        parameters[0] = value;
        value = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (value)
            value->value = 10;
        parameters[1] = value;
        ParameterValue *end = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (end)
            end = func_00175648(end);
        parameters[2] = end;
        func_001754d0(menu->third_pairs, parameters);
        ReleaseParameters(parameters);
    }
    if (menu->last_values) {
        ParameterValue *parameters[2];
        ParameterValue *value = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (value)
            value->value = 5;
        parameters[0] = value;
        ParameterValue *end = static_cast<ParameterValue *>(
            func_0019ffa4(sizeof(ParameterValue), data_002232d5));
        if (end)
            end = func_001725e8(end);
        parameters[1] = end;
        func_0017245c(menu->last_values, parameters);
        ReleaseParameters(parameters);
    }
    if (menu->text) {
        MenuTextParameter *parameters[4];
        MenuTextParameter *parameter = static_cast<MenuTextParameter *>(
            func_0019ffa4(sizeof(MenuTextParameter), data_002232d5));
        parameters[0] = parameter ? func_00174f04(parameter, func_00172008(-1),
                                                0x210, 3, -120.0f, 8.0f, 2.0f) : 0;
        parameter = static_cast<MenuTextParameter *>(
            func_0019ffa4(sizeof(MenuTextParameter), data_002232d5));
        parameters[1] = parameter ? func_00174f04(parameter, func_00172008(-1),
                                                0x210, 3, -120.0f, -82.0f, 2.0f) : 0;
        parameter = static_cast<MenuTextParameter *>(
            func_0019ffa4(sizeof(MenuTextParameter), data_002232d5));
        parameters[2] = parameter ? func_00174f04(parameter, reinterpret_cast<const unsigned short *>(L""), 0x210, 3,
                                                -128.0f, -36.0f, 2.0f) : 0;
        parameter = static_cast<MenuTextParameter *>(
            func_0019ffa4(sizeof(MenuTextParameter), data_002232d5));
        parameters[3] = parameter ? static_cast<MenuTextParameter *>(func_00174ef8(parameter)) : 0;
        func_00174df0(menu->text, parameters);
        ReleaseParameters(parameters);
    }
    if (menu->groups) {
        ArrayOwner *groups[7];
        ArrayOwner *owner = static_cast<ArrayOwner *>(
            func_0019ffa4(sizeof(ArrayOwner), data_002232d5));
        if (owner)
            owner = func_00174cc8(owner, 0, 1,
            static_cast<MenuParameterObject *>(
                new MenuPlacementParameter(3, 0, 120.0f, -98.0f, 2.0f, 0x1000017)));
        groups[0] = owner;
        owner = static_cast<ArrayOwner *>(
            func_0019ffa4(sizeof(ArrayOwner), data_002232d5));
        groups[1] = owner ? func_00174cc8(owner, 1, 2,
            static_cast<MenuParameterObject *>(
                new MenuPlacementParameter(17, 0, 30.0f, 63.0f, 2.0f)),
            static_cast<MenuParameterObject *>(
                new MenuPlacementParameter(18, 1, 99.0f, 63.0f, 2.0f))) : 0;
        owner = static_cast<ArrayOwner *>(
            func_0019ffa4(sizeof(ArrayOwner), data_002232d5));
        groups[2] = owner ? func_00174cc8(owner, 2, 2,
            static_cast<MenuParameterObject *>(
                new MenuPlacementParameter(17, 0, 30.0f, 23.0f, 2.0f)),
            static_cast<MenuParameterObject *>(
                new MenuPlacementParameter(18, 1, 99.0f, 23.0f, 2.0f))) : 0;
        owner = static_cast<ArrayOwner *>(
            func_0019ffa4(sizeof(ArrayOwner), data_002232d5));
        if (owner) {
            MenuPlacementParameter *storage = static_cast<MenuPlacementParameter *>(
                func_0019d788(sizeof(MenuPlacementParameter)));
            MenuParameterObject *parameter;
            if (storage) {
                parameter = func_00174d9c(storage, 0x1c, 1,
                                          65.0f, -16.0f, 2.0f, 0x1000016);
            } else {
                parameter = 0;
            }
            owner = func_00174cc8(owner, 3, 1, parameter);
        }
        groups[3] = owner;
        owner = static_cast<ArrayOwner *>(
            func_0019ffa4(sizeof(ArrayOwner), data_002232d5));
        if (owner) {
            MenuParameterObject *last = new MenuPlacementParameter(
                0x81, 2, 127.0f, -53.0f, 2.0f);
            MenuPlacementParameter *middle_storage = static_cast<MenuPlacementParameter *>(
                func_0019d788(sizeof(MenuPlacementParameter)));
            MenuParameterObject *middle = middle_storage ? func_00174d9c(
                middle_storage, 0x80, 1, 127.0f, 12.0f, 2.0f, 0) : 0;
            owner = func_00174cc8(owner, 4, 3, static_cast<MenuParameterObject *>(
                new MenuPlacementParameter(0x7f, 0, 127.0f, 77.0f, 2.0f)), middle, last);
        }
        groups[4] = owner;
        owner = static_cast<ArrayOwner *>(
            func_0019ffa4(sizeof(ArrayOwner), data_002232d5));
        groups[5] = owner ? func_00174cc8(owner, 5, 3,
            static_cast<MenuParameterObject *>(
                new MenuPlacementParameter(114, 0, 100.0f, -105.0f, 2.0f, 0x1000016)),
            static_cast<MenuParameterObject *>(
                new MenuPlacementParameter(115, 1, 0.0f, -105.0f, 2.0f, 0x1000019)),
            static_cast<MenuParameterObject *>(
                new MenuPlacementParameter(116, 2, -100.0f, -105.0f, 2.0f, 0x1000016))) : 0;
        owner = static_cast<ArrayOwner *>(
            func_0019ffa4(sizeof(ArrayOwner), data_002232d5));
        if (owner)
            owner = reinterpret_cast<ArrayOwner *>(func_00174cb0(
            reinterpret_cast<W11HandleC *>(owner)));
        groups[6] = owner;
        func_001748ec(menu->groups, groups);
        ReleaseGroups(groups);
    }
    if (!menu->display)
        menu->display = func_001819f4();
    if (menu->display) {
        func_00181994(menu->display);
        func_0018191c(menu->display);
        func_0016dc94(menu->display, 0);
    }
    func_00186598();
    func_00177b10(reinterpret_cast<CallbackTarget *>(menu), data_00218680);
}
