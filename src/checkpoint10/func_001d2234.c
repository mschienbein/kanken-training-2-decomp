/* [INFERENCE] Descriptive type, matching the prior wrapper candidate
 * scratch/authors/w18e/src/func_001d222c.c. The wrapper selects linked;
 * this body clears both sides of that relationship. */
struct LinkedView {
    struct LinkedView *peer;
    unsigned char reserved04[0x114];
    struct LinkedView *linked;
};

void func_001d2234(struct LinkedView *view)
{
    if (view->peer) {
        if (view->peer->linked == view)
            view->peer->linked = 0;
        if (view->peer)
            view->peer = 0;
    }
}
