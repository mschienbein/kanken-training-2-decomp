#ifndef GAME_COMMAND_STATE_H
#define GAME_COMMAND_STATE_H

/* [INFERENCE] Command/parameter roles; the fields and constructor stores are
 * evidenced. Distinct object identities are not equated just for sharing offsets. */
struct W11HandleA { int id; };
struct W11HandleB { int id; };
struct W11HandleC { int id; unsigned first; unsigned second; };
struct W11SingleState { int status; };
struct W11ControlState { unsigned char reserved00[8]; int status08; int status0c; };
struct ParameterValue { int value; };
struct VectorParameter { int kind, first, second; float x, y, z; int extra; };
struct ParameterRecord { int kind, first, second, third; float x, y, z; };

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
extern struct W11HandleC *func_00174cb0(struct W11HandleC *object);
extern void func_00186598(void);
extern void func_00181a4c(int kind, int first, int second, int value, unsigned flags);
#ifdef __cplusplus
}
#endif
#endif
