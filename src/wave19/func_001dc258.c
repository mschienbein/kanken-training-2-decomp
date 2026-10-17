#include "game/notification.h"

struct NotificationHeaderCopy {
    unsigned words[5];
};

void func_001dc258(int index, const struct NotificationHeaderCopy *header)
{
    unsigned *destination;
    if (index == -1)
        destination = bss_00280af8.storage.words;
    else
        destination = bss_002476b0[index].storage.words;
    destination[0] = header->words[0];
    destination[1] = header->words[1];
    destination[2] = header->words[2];
    destination[3] = header->words[3];
    destination[4] = header->words[4];
}
