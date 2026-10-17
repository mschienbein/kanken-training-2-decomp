/* [INFERENCE] Descriptive ownership types; the preceding tail wrapper
 * takes its link from the owner's field at 0x3040. These match the types
 * in scratch/authors/w18i/src/func_001d033c.c, not a proven class identity. */
struct LinkedOwner;

struct OwnedLink {
    struct LinkedOwner *owner;
};

struct LinkedOwner {
    unsigned char reserved00[0x3040];
    struct OwnedLink *link;
};

void func_001d0348(struct OwnedLink *link)
{
    if (link->owner) {
        if (link->owner->link == link)
            link->owner->link = 0;
        if (link->owner)
            link->owner = 0;
    }
}
