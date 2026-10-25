#include "game/property_text.h"

/* [INFERENCE] Non-owning lookup view; this is source-level factoring, not a
 * recovered historical class identity. */
struct PropertyLookup {
    explicit PropertyLookup(const PropertyTable *p) : table(p) {}
    const PropertyTable *table;
    const char *find(const char *name) const
    {
        unsigned count = table->count;
        for (unsigned i = 0; count > i; ++i)
            if (func_0018f860(table->entries[i].name, name, 0x40) == 0)
                return table->entries[i].value;
        return 0;
    }
};

Text64 func_00112fb8(const PropertyReference *reference)
{
    Text64 result = {{0}};
    const char *text = PropertyLookup(reference->table).find("sys.ECardTypeCode");
    if (text)
        func_0018f7f4(result.text, text, sizeof(result.text));
    return result;
}
