#include "game/object_manager.h"

struct ExtendedOperationPair {
    unsigned first;
    unsigned second;
};
struct ExtendedOperationSettings {
    struct ExtendedOperationPair pair;
    unsigned third;
};
struct ExtendedOperationTransform {
    float first[8];
    float last[5];
};
struct ExtendedOperation {
    unsigned char reserved00[0x50];
    struct RegistryEntry manager_entry;
    unsigned char reserved5c[0x108];
    unsigned char state;
    unsigned char reserved165;
    unsigned char linked;
    unsigned char reserved167;
    struct ExtendedOperationSettings settings;
    unsigned char reserved174[8];
    struct ExtendedOperationTransform transform;
};


void func_001511b0(struct ExtendedOperation *operation,
                   const struct ExtendedOperationSettings *settings,
                   const struct ExtendedOperationTransform *transform)
{
    struct ExtendedOperationPair pair = settings->pair;
    operation->settings.third = settings->third;
    operation->settings.pair = pair;
    {
        float a = transform->first[0];
        float b = transform->first[1];
        float c = transform->first[2];
        float d = transform->first[3];
        float e = transform->first[4];
        float f = transform->first[5];
        float g = transform->first[6];
        float h = transform->first[7];
        operation->transform.first[0] = a;
        operation->transform.first[1] = b;
        operation->transform.first[2] = c;
        operation->transform.first[3] = d;
        operation->transform.first[4] = e;
        operation->transform.first[5] = f;
        operation->transform.first[6] = g;
        operation->transform.first[7] = h;
    }
    {
        float a = transform->last[0];
        float b = transform->last[1];
        float c = transform->last[2];
        float d = transform->last[3];
        float e = transform->last[4];
        operation->transform.last[0] = a;
        operation->transform.last[1] = b;
        operation->transform.last[2] = c;
        operation->transform.last[3] = d;
        operation->transform.last[4] = e;
    }
    operation->state = 1;
    func_0017e640(func_00185d1c(), &operation->manager_entry);
    operation->linked = 1;
}
