#ifndef GAME_WINDOW_CONFIGURATION_H
#define GAME_WINDOW_CONFIGURATION_H


#include "game/runtime.h"
#include "game/resource_descriptors.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
namespace S2073fc {
class WindowMotion {
public:
    WindowMotion(int mode, float mode_offset, int screen, float screen_offset,
                 int first, float first_offset, int second, float second_offset,
                 int third, float third_offset);
private:
    int mode;
    int screen;
    int related[3];
    float offsets[5];
};



}
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern S2073fc::WindowMotion *data_002468c4[41];
extern ResourceDescriptor *data_00246968[48];
extern ResourceDescriptor *data_00246a28[48];
extern void func_002073fc(void);
#ifdef __cplusplus
}
#endif

#endif
