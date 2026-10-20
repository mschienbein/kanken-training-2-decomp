#include <preinclude.h>
#include "nw/snd/snd_WaveArchiveFileReader.h"

void nw::snd::internal::WaveArchiveFileReader::Reset()
{
    m_pHeader = 0;
    m_pInfoBlockBody = 0;
    m_pLoadTable = 0;
    m_Initialized = 0;
}
