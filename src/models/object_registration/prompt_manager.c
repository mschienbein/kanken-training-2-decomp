#include "game/functions.h"

/* [INFERENCE] Source-only typed no-op registration. The original call identity
 * was erased; no callback is stored or invoked and no helper earns credit. */
void register_prompt_manager_cleanup(struct PromptManager *object,
    struct PromptManager *(*destroy)(struct PromptManager *),
    void (*image_base)(void))
{
}
