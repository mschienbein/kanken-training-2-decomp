#ifndef GAME_SCENE_DISPATCH_H
#define GAME_SCENE_DISPATCH_H

#include "game/resource_runtime.h"

#ifdef __cplusplus
/* [INFERENCE] Each exact global is initialized through the common resource
 * constructor and dispatched through its vtable. Only that prefix is exposed;
 * this is not a declaration of each complete static object's extent. */

extern "C" {

extern ScreenResourceManager bss_0028e848;
extern ScreenResourceManager bss_0028e1e0;
extern ScreenResourceManager bss_0028f080;
extern ScreenResourceManager bss_0028dfa4;
extern ScreenResourceManager bss_0028ecf4;
extern ScreenResourceManager bss_0028e318;
extern ScreenResourceManager bss_0028e790;
extern ScreenResourceManager bss_0028e404;
extern ScreenResourceManager bss_0028e27c;
extern ScreenResourceManager bss_0028f3d8;
extern ScreenResourceManager bss_0028db2c;
extern ScreenResourceManager bss_0028f850;
extern ScreenResourceManager bss_0028d9a4;
extern ScreenResourceManager bss_0028da50;
extern ScreenResourceManager bss_0028e900;
extern ScreenResourceManager bss_0028f200;
extern ScreenResourceManager bss_0028fdd4;
extern ScreenResourceManager bss_0028fe70;
extern ScreenResourceManager bss_0028ff10;
extern ScreenResourceManager bss_0028fd24;
extern ScreenResourceManager bss_0028dd7c;
}
#endif

#endif
