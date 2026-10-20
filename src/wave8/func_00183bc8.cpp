#include <preinclude.h>
#include "nw/snd/snd_WaveArchiveFileReader.h"

const void *nw::snd::internal::WaveArchiveFileReader::SetWaveFile(
    unsigned index, const void *replacement)
{
    if (m_Initialized && m_pLoadTable) {
        if (index < m_pInfoBlockBody->table.count) {
            const void *previous = m_pLoadTable->waveFile[index];
            m_pLoadTable->waveFile[index] = replacement;
            return previous;
        }
    }
    return 0;
}
