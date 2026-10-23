#ifndef GAME_FILE_OPEN_H
#define GAME_FILE_OPEN_H
#include "game/service_protocol.h"
#include "game/service_result.h"
/* [INFERENCE] Opaque caller-forwarded options; no fabricated layout. */
struct FileOpenOptions;
extern "C" {
Result func_0011fca0(ServiceObject *object, unsigned offset, unsigned size,
                   const FileOpenOptions *options);
ServiceResult func_00112ae0(ServiceObject *object, unsigned offset,
                          unsigned size, const FileOpenOptions *options);
}
#endif
