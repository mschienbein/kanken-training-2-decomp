#include "game/archive.h"

const struct BankWaveArchiveTable *func_00185a6c(const struct BankReader *reader)
{
    if (!reader->valid)
        return 0;
    return func_00140fc0(reader->info);
}
