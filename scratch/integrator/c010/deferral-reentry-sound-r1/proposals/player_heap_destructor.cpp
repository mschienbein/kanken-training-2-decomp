/* [INFERENCE] Native owner re-entry: the existing native constructor and
 * Finalize provider establish PlayerHeapDataManager/SoundArchiveLoader bases.
 * This is a private typed proposal, not an admitted identity or ledger unit. */
#include <preinclude.h>
#include <nw/snd/snd_PlayerHeapDataManager.h>

namespace nw { namespace snd { namespace internal {
inline void PlayerHeapDataManager::Finalize()
{
    if (m_IsFinalized)
        return;
    m_IsInitialized = false;
    m_IsFinalized = true;
    SetSoundArchive(NULL);
}

PlayerHeapDataManager::~PlayerHeapDataManager()
{
    Finalize();
}
} } }
