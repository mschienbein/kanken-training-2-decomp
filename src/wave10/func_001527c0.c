struct PrefetchStreamInfo {
    unsigned char encoding;
    unsigned char loop;
    unsigned char channel_count;
    unsigned char reserved03;
    unsigned sample_rate;
    unsigned loop_start;
    unsigned sample_count;
    unsigned block_count;
    unsigned block_size;
    unsigned samples_per_block;
    unsigned final_block_size;
    unsigned final_block_samples;
    unsigned final_block_padded_size;
    unsigned reserved28[7];
    unsigned seek_interval;
    unsigned seek_entry_size;
};
struct PrefetchTrackInfo {
    unsigned char volume;
    unsigned char pan;
    unsigned char surround_pan;
    unsigned char lowpass;
    unsigned char send_a;
    unsigned char send_b;
    unsigned char send_c;
    unsigned char extra7;
    unsigned char extra8;
    unsigned char extra9;
    unsigned char channel_count;
    unsigned char channel_indices[2];
};
struct StreamPlaybackInfo {
    unsigned char encoding;
    unsigned char reserved01[3];
    unsigned sample_rate;
    unsigned char loop;
    unsigned char reserved09[3];
    unsigned loop_start;
    unsigned sample_count;
    unsigned seek_interval;
    unsigned seek_entry_size;
    unsigned samples_per_block;
    unsigned block_size;
    unsigned final_block_padded_size;
    unsigned final_block_samples;
    unsigned channel_count;
    unsigned track_count;
    struct PrefetchTrackInfo tracks[4];
};
extern unsigned func_0017e184(unsigned encoding);

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
