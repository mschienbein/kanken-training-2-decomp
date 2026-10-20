#include "game/notification.h"

void func_001db9b0(unsigned int course, int *grade, int *stage, int *progress)
{
    const struct NotificationFields *payload = &bss_00280af8.storage.fields;
    if (course < 2) {
        const struct W10CourseProgress *entry = &payload->courses[course];
        if (grade)
            *grade = entry->grade;
        if (stage)
            *stage = entry->stage;
        if (progress)
            *progress = entry->progress;
    }
}
