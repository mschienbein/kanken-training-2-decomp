#ifndef GAME_COMMAND_STATE_H
#define GAME_COMMAND_STATE_H

#include "game/property_descriptor.h"

/* [INFERENCE] Command/parameter roles; the fields and constructor stores are
 * evidenced. Distinct object identities are not equated just for sharing offsets. */
struct W11HandleA { int id; };
struct W11HandleB { int id; };
struct ArrayOwner { int index, count; void *elements; };
struct W11SingleState { int status; };
struct W11ControlState { unsigned char reserved00[8]; int status08; int status0c; };
struct ParameterValue { int value; };
struct VectorParameter { int kind, first, second; float x, y, z; const wchar_t *extra; };
struct ParameterRecord { int kind, first, second, third; float x, y, z; };

struct CommandResetContext;
#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] The wrappers forward sentinel-terminated NameTable pointer tables
 * to the common dispatcher. A negative count selects its sentinel-limited scan.
 * Descriptor category/identifier words are read; wrappers ignore the result. */
extern void func_0017a7e4(const struct NameTable *const *table, int count,
                          int mode, unsigned flags);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
struct W11BaseCommand {
    W11BaseCommand();
    virtual ~W11BaseCommand();
};
struct W11PositionCommand : W11BaseCommand {
    W11PositionCommand();
    W11PositionCommand(int resource, int kind, unsigned option, float x, float y, float z);
    virtual ~W11PositionCommand();
    short resource;
    short kind;
    unsigned short flags;
    unsigned short reserved0a;
    float position[3];
    unsigned option;
};
extern "C" {
#endif
extern struct W11HandleA *func_001721d8(struct W11HandleA *object);
extern struct W11HandleB *func_001748ac(struct W11HandleB *object);
extern struct ArrayOwner *func_00174cb0(struct ArrayOwner *object);
extern void func_00186598(void);
extern void func_00181a4c(int kind, int first, int second, int value, unsigned flags);
#ifdef __cplusplus
}
#endif
#endif
