#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include "game/pool_list.h"

/* Cross-unit interfaces without a more specific subsystem header. */
struct ObjectManager;
struct AssetSlot;
struct AudioBiquadCoefficients;
struct AudioChannelState;
struct AudioModulationState;
struct BinaryResourceReference;
struct CharacterPanel;
struct Component0015eb90;
struct Component001acc1c;
struct Component001acebc;
struct Component001ad134;
struct Component001adff8;
struct Component001af244;
struct Component001b4718;
struct Component001b6b88;
struct Component001b7c88;
struct Component001eaaac;
struct Component001eaf70;
struct CountedResource;
struct DeferredRequest;
struct DetailEntry;
struct ExtendedWorkspace;
struct FlagDisplayObject;
struct KernelHandleOwner;
struct MotionState;
struct NavigationState;
struct W10DrawingInput;
struct NotificationPayload;
struct Heap;
struct ServiceCoordinator;

/* Domain entries use the same count-and-sentinel intrusive list as pool users. */


struct RegistryEntry {
    const void *type;
    struct PoolLink link;
};

/* Shared by the timing reset and the object that embeds its state. */
struct TimingState {
    float elapsed;
    float duration;
    unsigned int count;
    unsigned char active;
    unsigned char paused;
    unsigned char complete;
};

/* Generic destructor registration erases the concrete object-pointer type.
 * Typed destructor callbacks are converted explicitly at the registration site;
 * all object pointers use the same single-word AAPCS parameter ABI. */
typedef void (*ObjectDestructor)(void *object);

#ifdef __cplusplus
extern "C" {
#endif

extern void func_00100000(void);
extern int func_00191c80(volatile unsigned int *guard);
extern int func_00122500(void);
extern void func_00122510(void);
extern void *func_00186488(void *object);
extern void func_00186fbc(unsigned *state);
extern void *func_00125b54(void *object);
extern unsigned func_001424c0(void);
extern unsigned func_001424d0(void);
extern int func_00144224(int value);
extern void func_0014f7e8(unsigned unused0, unsigned unused1);
extern void func_0014c2c8(void);
extern void func_00147260(struct ObjectManager *object);
extern struct PoolLink *func_0018d868(struct PoolList *registry, struct PoolLink *node);
extern void func_0017e7bc(struct PoolList *registry, struct RegistryEntry *entry);
extern struct PoolLink *func_0017e790(struct PoolList *registry, struct RegistryEntry *entry);
extern struct ObjectManager *func_00185d1c(void);
extern struct PoolList *func_0017e7c4(void);
extern void func_0017e818(struct TimingState *state);
extern unsigned func_0019d048(void);
extern unsigned func_0019d058(void);
extern void *func_001c5b88(void *object);
extern void func_001c6adc(void);
extern void func_001db134(void *object);
extern void func_00215ec4(volatile unsigned int *guard);
extern void func_00216140(void *object, ObjectDestructor destroy, void (*image_base)(void));

extern void func_001091d4(int, int, int, int);
extern void func_0010920c(int);
extern void func_0010a804(int);
extern void func_0010a828(int);
extern void func_0010aa18(int);
extern void func_0010b5f0(void *);
extern int func_0010b610(void *);
extern void func_0010ce44(void (*)(void *), void *);
extern void func_0010ce58(int (*)(void *), void *);
extern void func_0010cf84(void (*)(void *), void *);
extern void func_0010e818(void);
extern void func_00110a58(void);
extern int func_00113404(const unsigned int *, unsigned int);
extern void func_00114fa8(void);
extern unsigned char func_00115118(void);
extern int func_00115128(void);
extern void func_00116ca4(struct Heap *);
extern void func_00116cd8(struct Heap *);
extern void func_0011c77c(void);
extern void func_0011d2d4(void);
extern void func_0011da90(unsigned int, unsigned int);
extern void func_0011db78(int);
extern unsigned int func_00122890(unsigned int, void *, unsigned int);
extern unsigned int func_00122894(unsigned int, void *, unsigned int);
extern signed char func_001228d4(unsigned int);
extern unsigned int func_00122a20(void);
extern void func_00122a68(void);
extern void func_00122ad0(void);
extern void func_0012d2bc(void);
extern void func_0012d2e8(void);
extern void func_0012d328(void);
extern int func_0012f980(void);
extern int func_0012f9fc(unsigned int, const void *, unsigned int);
extern void func_001351bc(void);
extern void func_00135268(unsigned int, int);
extern void func_00135a28(void);
extern void func_00135a58(void);
extern void func_00135a88(void);
extern void func_00135ab8(void);
extern void func_00135ae8(void);
extern void func_00135b4c(int);
extern void func_00135bb0(int);
extern void func_0013608c(unsigned char, unsigned char, unsigned char);
extern void func_001360a4(void);
extern void func_001360ec(int, int);
extern void func_00136104(void);
extern void func_00136134(int, int);
extern unsigned int func_0013776c(void *, unsigned int);
extern unsigned int func_001378d8(const void *);
extern int func_0013e6b8(void);
extern unsigned int func_0013e6d0(unsigned int, unsigned int);
extern int func_00145f74(void (*)(void), void (*)(void), void (*)(void));
extern void func_00147998(unsigned int);
extern float func_001493a8(float);
extern float func_001493c4(float);
extern void func_00149424(void);
extern float func_00149428(float);
extern struct W10DrawingInput *func_00149a98(struct W10DrawingInput *);
extern unsigned int func_0014c338(void);
extern unsigned int func_0014c33c(void);
extern void func_00152664(struct AudioChannelState *, const struct AudioBiquadCoefficients *);
extern int func_001549c8(void);
extern int func_00156f04(void);
extern int func_00156f50(void);
extern void func_0015cab4(void);
extern void func_0015ea60(struct FlagDisplayObject *, int, int);
extern int func_0015eb90(struct Component0015eb90 *);
extern int func_0015f5b0(const struct CountedResource *);
extern void func_00166f1c(void *, int, int);
extern int func_00169974(void);
extern void func_00169b58(void);
extern int func_0016d000(int);
extern void func_0016d310(struct W10DrawingInput *);
extern int func_0016dc50(struct CharacterPanel *, int);
extern void func_0016f024(struct MotionState *);
extern void func_0016f55c(int, int, unsigned int, unsigned int, unsigned int, float, float);
extern void func_0016fa18(int);
extern void func_0016fa38(int);
extern int func_00174430(void);
extern int func_00178e9c(struct NavigationState *, int, int);
extern int func_00179ed4(struct CharacterPanel *, int);
extern void func_0017e8bc(struct ExtendedWorkspace *);
extern void func_00180314(int);
extern void func_00180e14(struct ServiceCoordinator *, float);
extern struct NotificationPayload *func_00181224(int);
extern void func_00181250(int);
extern struct W10DrawingInput *func_001819f4(void);
extern void func_00181dcc(int, int, int);
extern unsigned int func_00182b5c(unsigned int, unsigned int);
extern void func_00182db4(unsigned int, unsigned int, int);
extern void func_001840dc(struct BinaryResourceReference *);
extern struct DetailEntry *func_001851f0(int, int);
extern void func_001864cc(unsigned char);
extern void func_001865ec(int, int);
extern void func_00186944(int, int);
extern int func_001869d0(int);
extern void func_001869f0(void);
extern unsigned char func_001923c4(void);
extern void func_00193af4(unsigned int);
extern int func_00195e94(void);
extern void func_0019b574(unsigned int, unsigned int *);
extern int func_0019ceb4(void);
extern void func_0019d95c(unsigned int, int, int);
extern void func_0019eb24(void);
extern void func_0019eb4c(void);
extern void func_0019eb60(int, int);
extern int func_0019f2e8(void);
extern int func_001acc1c(struct Component001acc1c *);
extern int func_001acebc(struct Component001acebc *);
extern int func_001ad134(struct Component001ad134 *);
extern void func_001adff8(struct Component001adff8 *);
extern int func_001af244(struct Component001af244 *);
extern int func_001b4718(struct Component001b4718 *);
extern int func_001b6b88(struct Component001b6b88 *);
extern int func_001b7c88(struct Component001b7c88 *);
extern void *func_001c0e44(void *);
extern void func_001c31c0(struct AssetSlot **);
extern void func_001c3618(struct AssetSlot **);
extern void func_001c3854(struct AssetSlot **);
extern void func_001c42e8(struct AssetSlot **);
extern struct KernelHandleOwner *func_001c526c(struct KernelHandleOwner *);
extern int func_001c55dc(int);
extern int func_001c7a1c(void);
extern void func_001d98f4(struct AudioModulationState *);
extern void func_001dc064(int);
extern void func_001dc084(int);
extern void func_001dc0a4(int);
extern void func_001dc3a4(int);
extern void func_001e358c(struct DeferredRequest *);
extern int func_001eaaac(struct Component001eaaac *);
extern int func_001eaf70(struct Component001eaf70 *);
extern void func_001eb1cc(struct W10DrawingInput *, unsigned int);
extern void func_00176dc8(int index);
extern void func_00181e44(void);
extern void func_001acc18(void *object);
extern int func_00177124(int mode);
extern void func_0016d4f8(struct W10DrawingInput *controller);
extern void func_0016fca0(int mode);

#ifdef __cplusplus
}
#endif

#endif
