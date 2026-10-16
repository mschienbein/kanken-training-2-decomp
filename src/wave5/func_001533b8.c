#include "game/archive.h"

struct RelativeReference {
    unsigned short type;
    unsigned short reserved;
    int offset;
};
struct ResourceBlock {
    const struct ArchiveFile *file;
    const struct RelativeReference *reference;
};
extern const void *func_0017ded4(const struct RelativeReference *reference);
extern void func_00192720(void *destination, const void *source, unsigned size);

int func_001533b8(const struct ResourceBlock *block, void *output)
{
    const void *source = func_0017ded4(block->reference);
    func_00192720(output, source, 0x4c);
    return 1;
}
