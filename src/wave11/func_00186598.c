#include <stddef.h>

struct W11QueueTail {
    unsigned char reserved00[0x20];
    unsigned update;
    int write_index;
    int read_index;
};

struct W11Queue {
    unsigned active;
    unsigned reserved04;
    unsigned pending;
    unsigned char reserved0c[0xff4];
    struct W11QueueTail tail;
};

struct W11WakeEvent;
extern struct W11QueueTail bss_0049a0d0;
extern struct W11WakeEvent bss_0049a0e8;
extern void func_0019ea6c(struct W11WakeEvent *event);

void func_00186598(void)
{
    struct W11QueueTail *tail = &bss_0049a0d0;
    struct W11Queue *queue = (struct W11Queue *)((unsigned char *)tail - offsetof(struct W11Queue, tail));
    queue->tail.update = 0;
    if (queue->active && tail->write_index != tail->read_index && !queue->pending) {
        queue->pending = 1;
        func_0019ea6c(&bss_0049a0e8);
    }
}
