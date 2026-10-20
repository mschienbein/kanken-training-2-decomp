#include "game/functions.h"
#include "game/text_writer.h"
#include "game/screen.h"
#include "game/runtime.h"

#include "game/resource_runtime.h"



extern "C" void func_00176e24(struct MenuScene *manager)
{
    int i;
    /* [INFERENCE] callee identity: inlined by armlink. */
    if (manager->buffered) finish_scene_buffer(manager->buffered);
    if (manager->indexed) finish_scene_indexed(manager->indexed);
    if (manager->request) func_001e3548(manager->request);
    if (manager->options) func_001b3434(manager->options);
    if (manager->labels) func_001ea87c(manager->labels);
    if (manager->controls) func_001e9cdc(manager->controls);
    if (manager->style) func_001eaed8(manager->style);
    if (manager->primary_list) func_00168454(manager->primary_list);
    if (manager->selector) func_001af424(manager->selector);
    if (manager->handle_panel) func_001acff0(manager->handle_panel);
    if (manager->navigation) func_001682e8(manager->navigation);
    if (manager->scalar) func_001b6a0c(manager->scalar);
    if (manager->controller68) func_001b45ac(manager->controller68);
    if (manager->animation) func_001b41b8(manager->animation);
    if (manager->record_selection) func_001aeb30(manager->record_selection);
    if (manager->pair_selection) func_001acd38(manager->pair_selection);
    if (manager->message) func_001ad708(manager->message);
    if (manager->characters) func_001b5e44(manager->characters);
    if (manager->controller80) func_001b494c(manager->controller80);
    if (manager->indicators) func_001b7604(manager->indicators);
    if (manager->controller88) func_001ae140(manager->controller88);
    if (manager->transition) func_001b64b0(manager->transition);
    if (manager->first_count > 0 && manager->first) {
        for (i = 0; i < manager->first_count; ++i) {
            if (manager->first[i].object) {
                func_00183208(manager->first[i].object);
                manager->first[i].object = 0;
            }
        }
        func_0019dc20(manager->first);
        manager->first = 0;
    }
    if (manager->second_count > 0 && manager->second) {
        for (i = 0; i < manager->second_count; ++i) {
            if (manager->second[i].object) {
                func_00183208(manager->second[i].object);
                manager->second[i].object = 0;
            }
            if (manager->second[i].allocation) {
                func_0019dc20(manager->second[i].allocation);
                manager->second[i].allocation = 0;
            }
        }
        func_0019dc20(manager->second);
        manager->second = 0;
    }
    if (manager->third_count > 0 && manager->third) {
        for (i = 0; i < manager->third_count; ++i) {
            if (manager->third[i].object) {
                func_00183208(manager->third[i].object);
                manager->third[i].object = 0;
            }
            func_00180314(manager->third[i].texture_slot);
        }
        func_0019dc20(manager->third);
        manager->third = 0;
    }
}
