/* Adapted from ctrdecomp/NintendoWare4Ctr, GPL-3.0,
 * commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/lyt/lyt_Animation.cpp. Changes: standalone key type and address symbol. */
struct HermiteKey {
    float frame;
    float value;
    float slope;
};

static inline bool same_frame(float first, float second, float tolerance)
{
    float difference = first - second;
    return -tolerance < difference && difference < tolerance;
}

extern "C" float func_0016b898(float frame, const HermiteKey *keys, unsigned count)
{
    if (count == 1 || frame <= keys[0].frame)
        return keys[0].value;
    else if (frame >= keys[count - 1].frame)
        return keys[count - 1].value;

    unsigned low = 0;
    unsigned high = count - 1;
    while (low != high - 1 && low != high) {
        int middle = (low + high) / 2;
        if (frame <= keys[middle].frame)
            high = middle;
        else
            low = middle;
    }
    const HermiteKey &first = keys[low];
    const HermiteKey &second = keys[high];
    if (same_frame(frame, second.frame, 0.001f)) {
        if (high < count - 1 && second.frame == keys[high + 1].frame)
            return keys[high + 1].value;
        else
            return second.value;
    }
    float t1 = frame - first.frame;
    float t2 = 1.0f / (second.frame - first.frame);
    float v0 = first.value;
    float v1 = second.value;
    float s0 = first.slope;
    float s1 = second.slope;
    float t1t1t2 = t1 * t1 * t2;
    float t1t1t2t2 = t1t1t2 * t2;
    float t1t1t1t2t2 = t1 * t1t1t2t2;
    float t1t1t1t2t2t2 = t1t1t1t2t2 * t2;
    return v0 * (2.0f * t1t1t1t2t2t2 - 3.0f * t1t1t2t2 + 1.0f) +
           v1 * (-2.0f * t1t1t1t2t2t2 + 3.0f * t1t1t2t2) +
           s0 * (t1t1t1t2t2 - 2.0f * t1t1t2 + t1) +
           s1 * (t1t1t1t2t2 - t1t1t2);
}
