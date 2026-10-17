#include "game/functions.h"
#include "game/render_state.h"


extern void func_00126f68(unsigned *state);
int func_0011d2f0(struct SubmissionState *state)
{
    data_002429f0 = state;
    func_00127294(&data_002429f0->command_mode);
    func_00126f5c(&data_002429f0->enabled);
    func_00186fbc(&data_002429f0->texture_state);
    func_00126f68(&data_002429f0->feature_mask);
    return 0;
}
