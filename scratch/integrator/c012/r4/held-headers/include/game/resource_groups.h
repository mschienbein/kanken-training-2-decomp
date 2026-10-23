#ifndef GAME_RESOURCE_GROUPS_H
#define GAME_RESOURCE_GROUPS_H
#ifdef __cplusplus
/* Names-admission's constructor/destructor references bind these guarded
 * owners. [INFERENCE] Handle/input class names and historical membership. */
struct W22WidgetMask {
    int bit_count, word_count;
    unsigned mask[1];
    W22WidgetMask(int count)
    {
        bit_count = count;
        word_count = (count + 31) / 32;
        for (int i = 0; i < word_count; ++i)
            mask[i] = 0;
    }
};
struct W22Handle;
struct TouchFrame;
struct W22ResourceGroup : W22WidgetMask {
    W22Handle *handles[25];
    TouchFrame *inputs[12];
    W22ResourceGroup();
    ~W22ResourceGroup();
};
struct W22LargeHandle;
struct TouchFrame;
struct W22LargeResourceGroup : W22WidgetMask {
    W22LargeHandle *handles[27];
    unsigned reserved78[3];
    TouchFrame *inputs[2];
    W22LargeResourceGroup();
    ~W22LargeResourceGroup();
};

extern "C" {
W22ResourceGroup *func_001ada1c(void);
W22LargeResourceGroup *func_001b50bc(void);
}
#endif
#endif
