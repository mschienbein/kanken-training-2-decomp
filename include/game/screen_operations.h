#ifndef GAME_SCREEN_OPERATIONS_H
#define GAME_SCREEN_OPERATIONS_H

#include "game/w11_control.h"
#include "game/render_parameter.h"
#include "game/character_panel.h"

struct L041Dialog;
#ifdef __cplusplus
extern "C" {
#endif
void func_0017433c(int channel, int duration);
void func_00181b68(int transition, int direction, int enabled);
int func_00177124(int transition);
void func_0017a258(struct AnimationState *state, float finish, float start);
void func_00175d90(struct Screen *screen);
void func_00175d18(struct Screen *screen, struct RenderParameter *const *parameters);
void func_00172b38(struct L041Dialog *dialog);
void func_00172a14(struct L041Dialog *dialog, int mode);
void func_0016d038(struct W10DrawingInput *input);
unsigned short func_0016d2dc(const struct W10DrawingInput *input);
int func_0016d2fc(const struct W10DrawingInput *input);
void func_0016d4f8(struct W10DrawingInput *input);
int func_0016dc50(struct CharacterPanel *panel, int index);
int func_0016e03c(const struct CharacterPanel *panel, int index);
void func_0016e61c(struct CharacterPanel *panel, int selected, int character);
int func_00179ed4(struct CharacterPanel *panel, int compact);
#ifdef __cplusplus
}
#endif

#endif
