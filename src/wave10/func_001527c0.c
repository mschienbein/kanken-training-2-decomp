#include "game/prefetch_stream.h"

void func_001527c0(struct StreamPlaybackInfo *output, const struct PrefetchStreamInfo *input)
{
    output->encoding = func_0017e184(input->encoding);
    output->sample_rate = input->sample_rate;
    output->loop = input->loop != 0;
    output->loop_start = input->loop_start;
    output->sample_count = input->sample_count;
    output->seek_interval = input->seek_interval;
    output->seek_entry_size = input->seek_entry_size;
    output->samples_per_block = input->samples_per_block;
    output->block_size = input->block_size;
    output->final_block_samples = input->final_block_samples;
    output->final_block_padded_size = input->final_block_padded_size;
}
