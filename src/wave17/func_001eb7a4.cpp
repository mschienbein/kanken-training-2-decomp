#include "game/functions.h"
#include "game/resource_runtime.h"


void MenuScene::finish()
{
    MenuScene *manager = this;
    int index;
    if (manager->buffered != 0) func_001dcf4c(manager->buffered);
    if (manager->indexed != 0) func_001dcdd0(manager->indexed);
    if (manager->request != 0) func_001e358c(manager->request);
    if (manager->options != 0) func_001b3564(manager->options);
    if (manager->labels != 0) func_001eaaac(manager->labels);
    if (manager->controls != 0) func_001e9e7c(manager->controls);
    if (manager->style != 0) func_001eaf70(manager->style);
    if (manager->primary_list != 0) func_0015eb90(manager->primary_list);
    /* [INFERENCE] callee identity: inlined by armlink. Only an empty
     * object operation is recoverable for this component. */
    if (manager->selector != 0) func_001acc18(manager->selector);
    if (manager->handle_panel != 0) func_001ad134(manager->handle_panel);
    if (manager->navigation != 0) func_001acc1c(manager->navigation);
    if (manager->scalar != 0) func_001b6b88(manager->scalar);
    if (manager->controller68 != 0) func_001b4718(manager->controller68);
    if (manager->animation != 0) func_001b4310(manager->animation);
    if (manager->record_selection != 0) func_001af244(manager->record_selection);
    if (manager->pair_selection != 0) func_001acebc(manager->pair_selection);
    if (manager->message != 0) func_001ad7a0(manager->message);
    if (manager->characters != 0) func_001b6098(manager->characters);
    if (manager->controller80 != 0) func_001b4d58(manager->controller80);
    if (manager->indicators != 0) func_001b7c88(manager->indicators);
    if (manager->controller88 != 0) func_001adff8(manager->controller88);
    if (manager->transition != 0) func_001b6604(manager->transition);
    if (manager->first_count > 0) {
        index = 0;
        do {
            func_001ebb14(manager, &manager->first[index]);
            ++index;
        } while (index < manager->first_count);
    }
    if (manager->second_count > 0) {
        index = 0;
        do {
            func_001eb95c(manager, &manager->second[index]);
            ++index;
        } while (index < manager->second_count);
    }
    if (manager->third_count > 0) {
        index = 0;
        do {
            func_001ebca0(manager, &manager->third[index]);
            ++index;
        } while (index < manager->third_count);
    }
}
