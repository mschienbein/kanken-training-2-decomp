#ifndef GAME_LINKED_OWNERS_H
#define GAME_LINKED_OWNERS_H

/* [INFERENCE] Descriptive ownership records. The wrapper selects the link at
 * +0x3040 and the paired cleanup clears its backlink. Layout equality does not
 * establish a historical native class or relationship to LinkedView. */
struct LinkedOwner;
struct OwnedLink { struct LinkedOwner *owner; };
struct LinkedOwner {
    unsigned char reserved00[0x3040];
    struct OwnedLink *link;
};

/* [INFERENCE] The two endpoint roles use this same partial C view; their
 * historical native class identity is not recovered. */
struct LinkedView {
    struct LinkedView *peer;
    unsigned char reserved04[0x114];
    struct LinkedView *linked;
};

#ifdef __cplusplus
extern "C" {
#endif
void func_001d033c(struct LinkedOwner *owner);
void func_001d0348(struct OwnedLink *link);
void func_001d222c(struct LinkedView *view);
void func_001d2234(struct LinkedView *view);
#ifdef __cplusplus
}
#endif
#endif
