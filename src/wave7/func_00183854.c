#include "game/stream_manager.h"

unsigned int *func_00183854(struct StreamManager *manager, unsigned int count)
{
    unsigned int *result = 0;
    if (manager->full)
        return 0;
    if (manager->read_index > manager->write_index) {
        if (manager->write_index + count <= manager->read_index) {
            result = manager->buffer + manager->write_index;
            manager->write_index += count;
        }
    } else {
        if (manager->capacity >= manager->write_index + count) {
            result = manager->buffer + manager->write_index;
            manager->write_index += count;
        } else if (manager->read_index >= count) {
            manager->write_index = count;
            result = manager->buffer;
        }
    }
    if (result && manager->write_index == manager->read_index)
        manager->full = 1;
    return result;
}

