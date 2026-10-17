struct FeatureProperties {
    unsigned reserved00;
    unsigned flags;

    void set_component(int component, int enabled)
    {
        unsigned mask = 0;
        switch (component) {
        case 0: mask = 0x100; break;
        case 1: mask = 0x200; break;
        case 2: mask = 0x400; break;
        }
        if (enabled)
            flags |= mask;
        else
            flags &= ~mask;
    }
};

struct IndexedFeature {
    unsigned char reserved00[0x18];
    FeatureProperties properties;
    unsigned char reserved20[0x2c];
    int active;
    unsigned reserved50;
};

extern "C" void func_00177a84(IndexedFeature *features, int index, int component,
                              int enabled)
{
    IndexedFeature *feature = &features[index];
    FeatureProperties *properties = &feature->properties;
    if (!feature->active)
        return;
    properties->set_component(component, enabled);
}
