/* Probable PUX recognition state; vendor attribution is provisional. */
struct StrokeState {
    unsigned short current;
    unsigned short count;
    unsigned short accepted;
    unsigned short next;
    unsigned short flags;
};
extern struct StrokeState *data_0024385c;
extern struct StrokeState bss_004e07f0;

int func_0014dc28(void)
{
    data_0024385c = &bss_004e07f0;
    return 0;
}

void func_0015cab4(void)
{
    data_0024385c->current = 0;
    data_0024385c->count = 0;
    data_0024385c->flags = 0;
    data_0024385c->accepted = 0;
    data_0024385c->next = 0;
}

struct RecognitionState { char reserved[0xd0]; int option; };
extern struct RecognitionState bss_004e071c;
void func_0015c5a4(int option)
{
    bss_004e071c.option = option;
}
