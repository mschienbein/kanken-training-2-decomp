#ifndef GAME_MATERIAL_INIT_H
#define GAME_MATERIAL_INIT_H
/* The seven-color caller proves the 12-byte image constructor's packed-word
 * role. Replace the former private HwrIndexValue tag with this shared type;
 * [INFERENCE] MaterialInit/MaterialColor are descriptive retail identities. */
struct MaterialColor {
    unsigned packed;
#ifdef __cplusplus
    MaterialColor();
#endif
};
#ifdef __cplusplus
#include <preinclude.h>
#include <nw/lyt/lyt_Material.h>
class MaterialInit {
public:
    MaterialInit();
    virtual ~MaterialInit();
private:
    nw::lyt::AnimationList animations;
    MaterialColor colors[7];
    nw::lyt::internal::MatMemNums capacity;
    nw::lyt::internal::MatMemNums count;
    void *memory;
    char name[21];
    unsigned char flags;
    unsigned char padding[2];
    void Init();
};

#endif
#ifdef __cplusplus
extern "C" {
#endif
void func_0014b438(struct MaterialColor *value);
#ifdef __cplusplus
}
#endif
#endif
