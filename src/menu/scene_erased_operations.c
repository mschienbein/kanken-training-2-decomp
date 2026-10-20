#include "game/resource_runtime.h"
#include "game/text_writer.h"

/* [INFERENCE] Source-defined erased lifetime operations. The original NOP
 * sites establish only an empty operation on these actual receivers, not
 * the identity of MenuScene::~MenuScene or another extant destructor. */
void finish_scene_buffer(struct BufferedSceneState *component) {}
void finish_scene_indexed(struct IndexedSceneState *component) {}
void finish_text_writer_copy(struct TextWriterState *writer) {}
