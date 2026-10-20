#include "game/resource_descriptors.h"

ResourceDescriptor::ResourceDescriptor(int category, int resource, const char *name)
{
    this->category = category;
    this->resource = resource;
    this->name = name;
}
