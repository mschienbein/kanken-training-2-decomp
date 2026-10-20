#include "game/data_reader.h"



const void *func_001f1fb4(const DataReader *reader)
{
    if (reader->first && reader->second)
        return reader->cached;
    return reader->source->get_data();
}
