#include "game/audio_voice.h"
#include "game/functions.h"
struct GraphicsConverters {
    float (*wrap)(float value);
    float (*curve)(float value);
    void (*reset)(void);
    float (*step)(float value);
    float (*random)(void);
    unsigned char reserved14[0x1ec];
};
extern struct GraphicsConverters bss_004b8440;
extern void func_0019cecc(void *destination, unsigned int size);






void func_00147478(void)
{
    func_0019cecc(&bss_004b8440, 0x200);
    bss_004b8440.wrap = func_00149428;
    bss_004b8440.curve = func_001493c4;
    bss_004b8440.reset = func_00149424;
    bss_004b8440.step = func_001493a8;
    bss_004b8440.random = func_00149388;
}
