#include "game/text_writer.h"

/* [INFERENCE] Original NOP proves only an empty operation on the copied
 * writer, not the identity of the actual MenuScene destruction entry. */
void finish_text_writer_copy(struct TextWriterState *writer) {}
