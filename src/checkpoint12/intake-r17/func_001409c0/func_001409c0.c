struct AnimationHeader {
    unsigned char reserved00[8];
    unsigned short frame_count;
};

struct AnimationResource {
    unsigned char reserved00[12];
    const struct AnimationHeader *header;
    float frame;
};

unsigned func_001409c0(const struct AnimationResource *resource)
{
    return resource->header->frame_count;
}
