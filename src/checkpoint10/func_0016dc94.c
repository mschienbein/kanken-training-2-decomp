struct W8EffectState {
    unsigned char reserved00[0x89e];
    unsigned short mask;
};

struct EffectPresetMasks {
    unsigned packed_masks[3];
    unsigned short fallback;
};

struct W19EffectPresetBlock {
    unsigned char reserved00[0x4c];
    struct EffectPresetMasks masks;
};

extern const struct W19EffectPresetBlock data_002181c0;
extern void func_0014a9d8(unsigned flags);

static __inline void apply_mask(struct W8EffectState *effects, unsigned short mask)
{
    effects->mask = mask;
    func_0014a9d8(mask);
}

void func_0016dc94(struct W8EffectState *effects, int index)
{
    struct EffectPresetMasks presets = data_002181c0.masks;
    unsigned short mask = presets.fallback;
    if (index == 0) mask = presets.packed_masks[0];
    else if (index == 1) mask = presets.packed_masks[0] >> 16;
    else if (index == 2) mask = presets.packed_masks[1];
    else if (index == 3) mask = presets.packed_masks[1] >> 16;
    else if (index == 4) mask = presets.packed_masks[2];
    else if (index == 5) mask = presets.packed_masks[2] >> 16;
    apply_mask(effects, mask);
}
