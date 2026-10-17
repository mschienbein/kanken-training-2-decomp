#include "game/audio_voice.h"
#include "game/stream_manager.h"



void func_001838ec(struct AudioWorker *worker)
{
    worker->stopping = 1;
    func_00183900(&worker->queue, 0);
}
