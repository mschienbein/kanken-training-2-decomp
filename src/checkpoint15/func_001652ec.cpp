/* [INFERENCE] Two adjacent texture-coordinate routines share this orientation
 * table and vector representation; historical TU membership is not established. */
#include "game/texture_coordinates.h"

static Tu582Vector2 texture_dimensions(const unsigned short dimensions[2])
{
    unsigned width = dimensions[0];
    unsigned height = dimensions[1];
    float w = (float)width;
    float h = (float)height;
    Tu582Vector2 size = {{w, h}};
    return size;
}

void func_001652ec(Tu582Vector2 coordinates[4], const Tu582Vector2 *offset,
                              const unsigned short dimensions[2], unsigned orientation)
{
    const struct Tu582Orientation *mapping = &data_002436dc[orientation];
    unsigned first = mapping->axes[0];
    unsigned second = mapping->axes[1];
    Tu582Vector2 size = texture_dimensions(dimensions);
    float value;
    value = (float)(unsigned)mapping->corners[3][first];
    coordinates[1][first] = value;
    coordinates[3][first] = value;
    value = (float)(unsigned)mapping->corners[3][second];
    coordinates[2][second] = value;
    coordinates[3][second] = value;
    value = offset->x() / (size[first] * (float)(mapping->corners[2][first] - mapping->corners[3][first]))
            + (float)(unsigned)mapping->corners[3][first];
    coordinates[2][first] = value;
    coordinates[0][first] = value;
    value = offset->y() / (size[second] * (float)(mapping->corners[1][second] - mapping->corners[3][second]))
            + (float)(unsigned)mapping->corners[3][second];
    coordinates[1][second] = value;
    coordinates[0][second] = value;
}


