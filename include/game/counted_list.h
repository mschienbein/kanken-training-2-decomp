#ifndef GAME_COUNTED_LIST_H
#define GAME_COUNTED_LIST_H

/* [INFERENCE] Node and signed-count traversal contract retained for the c010
 * implementation. The allocator-owner wrapper is deferred independently. */
struct CountedNode { struct CountedNode *next; };
#ifdef __cplusplus
extern "C" {
#endif
int func_001f478c(const struct CountedNode *const *head);
#ifdef __cplusplus
}
#endif
#endif
