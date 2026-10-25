#ifndef GAME_TEXTURE_COORDINATES_H
#define GAME_TEXTURE_COORDINATES_H

struct Tu582Vector2 {
    float elements[2];
    float x() const { return elements[0]; }
    float y() const { return elements[1]; }
    float &operator[](unsigned axis) { return elements[axis]; }
    const float &operator[](unsigned axis) const { return elements[axis]; }
};
struct Tu582Orientation {
    unsigned char corners[4][2];
    unsigned char axes[2];
};

extern "C" {
extern Tu582Orientation data_002436dc[];
void func_001651b8(Tu582Vector2 coordinates[4], const Tu582Vector2 *offset,
                   const unsigned short dimensions[2], unsigned orientation);
void func_001652ec(Tu582Vector2 coordinates[4], const Tu582Vector2 *offset,
                   const unsigned short dimensions[2], unsigned orientation);
}
#endif
