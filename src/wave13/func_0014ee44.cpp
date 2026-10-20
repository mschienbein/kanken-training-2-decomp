#include <preinclude.h>
#include "nw/snd/snd_WaveArchiveFileReader.h"

/* S15: the original vector-constructor callback constructs a CWAR reader. */
nw::snd::internal::WaveArchiveFileReader::WaveArchiveFileReader()
    : m_pHeader(0), m_pInfoBlockBody(0), m_pLoadTable(0), m_Initialized(0)
{
}
