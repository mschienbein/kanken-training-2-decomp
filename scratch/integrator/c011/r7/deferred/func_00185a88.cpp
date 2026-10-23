#include "game/archive.h"

static const struct SoundReference *get_bank_info(const struct BankFile *file)
{
    /* ORIGINAL BUG: a valid header without the expected block still yields an invalid info pointer. */
    return &func_0014a440(file)->reference;
}

static void attach_bank_file(const struct BankFile *file, struct BankReader *reader)
{
    if (file != 0 && file->signature == 0x4b4e4243 && file->version - 0x01000000u <= 0x100) {
        reader->file = file;
        const SoundReference *info = get_bank_info(file);
        bool valid = file != 0;
        reader->valid = valid;
        reader->info = info;
    }
}

extern "C" struct BankReader *func_00185a88(struct BankReader *reader, const struct BankFile *file)
{
    reader->file = 0;
    reader->info = 0;
    reader->valid = 0;
    attach_bank_file(file, reader);
    return reader;
}
