#ifndef GAME_RENDER_STATE_H
#define GAME_RENDER_STATE_H

struct SlotBlock {
    unsigned words[8];
    struct SlotBlock *next;
    struct SlotBlock *previous;
};

struct SlotList {
    struct SlotBlock *head;
    unsigned head_count;
    struct SlotBlock *tail;
    unsigned tail_count;
};

struct AllocationSystem {
    void *(*allocate)(unsigned module, unsigned kind, unsigned id, unsigned size);
    void (*release)(unsigned module, unsigned kind, unsigned id, void *memory);
    unsigned next_id;
    unsigned next_group;
    void *memory;
};
extern struct AllocationSystem data_002429f4;

struct SubmissionState {
    unsigned command_mode;
    unsigned enabled;
    unsigned texture_state;
    unsigned feature_mask;
};
extern struct SubmissionState *data_002429f0;

struct VertexAttribute {
    unsigned enabled;
    unsigned components;
    unsigned format;
    unsigned stride;
    unsigned offset;
    unsigned flags;
};

struct VertexRange {
    unsigned scale;
    unsigned bias;
    unsigned clamp;
};

struct PipelineConfiguration {
    struct VertexAttribute attributes[16];
    unsigned char reserved180[8];
    float constants[16][4];
    struct VertexRange ranges[16];
};

struct PipelineStorage {
    unsigned char reserved00[0x800];
    struct PipelineConfiguration *configuration;
    unsigned char reserved804[0x50];
};

struct PipelineOwner {
    struct PipelineStorage *storage;
    unsigned ready;
};
extern struct PipelineOwner data_00242a14;

extern void func_00126f5c(unsigned *state);
extern void func_00127294(unsigned *state);
extern struct SlotBlock *func_0018ce64(void);

#endif
