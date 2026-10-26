#ifndef GAME_CAMERA_DEFAULTS_H
#define GAME_CAMERA_DEFAULTS_H

/* [INFERENCE] Descriptive camera fields; the initializer and8-element producer
 * establish the record roles and0x254-byte extent. */
struct CameraVector {
    float x;
    float y;
    float z;
};

struct CameraDefaults {
    struct CameraVector position;
    struct CameraVector target;
    struct CameraVector rotation;
    struct CameraVector up;
    float distance;
    float field_of_view;
    float width;
    float height;
    float top;
    float bottom;
    float left;
    float right;
    float near_plane;
    float far_plane;
    int projection;
    float viewport_x;
    float viewport_y;
    float viewport_width;
    float viewport_height;
    int mode;
    int screen_x;
    int screen_width;
    int screen_height;
    int screen_y;
    int flags;
    unsigned char reserved84[0x1d0];
};

struct CameraSelection {
    int active;
    int previous;
    float blend;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct CameraDefaults bss_00497720[8];
extern struct CameraSelection bss_004989c0;
void func_001026dc(void);
#ifdef __cplusplus
}
#endif
#endif
