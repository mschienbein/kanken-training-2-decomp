#include "game/archive.h"
static const struct SoundReference *get_bank_info(const struct BankFile *file)
{
    /* ORIGINAL BUG: a valid header without the expected block still yields an invalid info pointer. */
    return &func_0014a440(file)->reference;
}

extern "C" void func_0015276c(struct BankReader *reader, const struct BankFile *file)
{
    if (file == 0)
        return;
    if (file->signature == 0x4b4e4243 && file->version - 0x01000000u <= 0x100) {
        reader->file = file;
        const SoundReference *info = get_bank_info(file);
        reader->valid = 1;
        reader->info = info;
    }
}
