#ifndef GAME_DISPLAY_TARGET_H
#define GAME_DISPLAY_TARGET_H
struct DisplayTarget;
#ifdef __cplusplus
/* [INFERENCE] Class and hook names; the matched constructor establishes the
 * native virtual prefix and observed target dimensions/formats. */
class DisplayTarget {
public:
    DisplayTarget();
    virtual void Finalize();
    unsigned color_format;
    unsigned width;
    unsigned height;
    unsigned flags;
    unsigned depth_format;
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void func_00110358(struct DisplayTarget *target);
#ifdef __cplusplus
}
#endif

#endif
