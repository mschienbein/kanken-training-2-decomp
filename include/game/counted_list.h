#ifndef GAME_COUNTED_LIST_H
#define GAME_COUNTED_LIST_H

/* [INFERENCE] Descriptive node and signed-count contract. The wrapper passes
 * the address of its head word at +8; the count routine follows next pointers
 * without modifying that word. This is not a recovered native class identity. */
struct CountedNode { struct CountedNode *next; };
struct CountedList {
    unsigned reserved00[2];
    const struct CountedNode *first;
};
#ifdef __cplusplus
extern "C" {
#endif
int func_001f4784(const struct CountedList *list);
int func_001f478c(const struct CountedNode *const *head);
#ifdef __cplusplus
}
#endif
#endif
