/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned math ABI declarations and elementary value operations inferred
 * from uses in public NintendoWare4Ctr, commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd. No Nintendo SDK source consulted.
 * Named matrix fields alias row-major m[][] as the public NW4C math/*.ipp code
 * requires. Transform3 field order is [INFERENCE] from scale/rotate/translate
 * resource semantics; a compile success is not a layout or inline-code proof.
 * Non-elementary operations stay declarations and resolve to original code.
 */
#pragma once
#include "../nw/types.h"
#define NN_MATH_RAD_TO_FIDX(value) ((value) * (128.0f / 3.14159265358979323846f))
namespace nn { namespace math {
struct VEC2 {
    f32 x, y;
    VEC2();
    VEC2(f32 a, f32 b): x(a), y(b) {}
    VEC2& Set(f32 a, f32 b) { x=a; y=b; return *this; }
    operator f32*() { return &x; }
    operator const f32*() const { return &x; }
    VEC2& operator+=(const VEC2&);
    VEC2& operator-=(const VEC2&);
    VEC2& operator*=(f32);
};
struct VEC3 {
    f32 x, y, z;
    VEC3();
    VEC3(f32 a, f32 b, f32 c): x(a), y(b), z(c) {}
    VEC3& Set(f32 a, f32 b, f32 c) { x=a; y=b; z=c; return *this; }
    VEC3& Set(const VEC3& value) { *this = value; return *this; }
    operator f32*() { return &x; }
    operator const f32*() const { return &x; }
    VEC3& operator+=(const VEC3&);
    VEC3& operator-=(const VEC3&);
    VEC3& operator*=(f32);
    VEC3& operator/=(f32);
    f32 LengthSquare() const;
    f32 Length() const;
    VEC3& Normalize();
    bool IsZero() const;
};
VEC3 operator+(const VEC3&, const VEC3&);
VEC3 operator-(const VEC3&, const VEC3&);
VEC3 operator*(const VEC3&, f32);
VEC3 operator*(f32, const VEC3&);
bool operator==(const VEC3&, const VEC3&);
bool operator!=(const VEC3&, const VEC3&);
struct VEC4 {
    f32 x, y, z, w;
    VEC4();
    VEC4(f32 a, f32 b, f32 c, f32 d): x(a), y(b), z(c), w(d) {}
    VEC4& Set(f32 a, f32 b, f32 c, f32 d) { x=a; y=b; z=c; w=d; return *this; }
    operator f32*() { return &x; }
    operator const f32*() const { return &x; }
};
struct QUAT { f32 x, y, z, w; QUAT(); QUAT(f32, f32, f32, f32); };
struct MTX23 { union { f32 m[2][3]; f32 a[6]; struct { f32 _00,_01,_02,_10,_11,_12; } f; }; };
struct MTX33 { union { f32 m[3][3]; f32 a[9]; struct { f32 _00,_01,_02,_10,_11,_12,_20,_21,_22; } f; }; };
struct MTX34 {
    union { f32 m[3][4]; f32 a[12]; struct { f32 _00,_01,_02,_03,_10,_11,_12,_13,_20,_21,_22,_23; } f; };
    MTX34();
    MTX34(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);
    static const MTX34& Identity();
    operator f32*() { return a; }
    operator const f32*() const { return a; }
    f32* operator[](int i) { return m[i]; }
    const f32* operator[](int i) const { return m[i]; }
    VEC3 GetColumn(int) const;
    MTX34& SetColumn(int, const VEC3&);
    MTX34& SetupScale(const VEC3&);
};
struct MTX44 {
    union { f32 m[4][4]; f32 a[16]; struct { f32 _00,_01,_02,_03,_10,_11,_12,_13,_20,_21,_22,_23,_30,_31,_32,_33; } f; };
    MTX44();
    static const MTX44& Identity();
    operator f32*() { return a; }
    operator const f32*() const { return a; }
};
struct Transform2 { VEC2 scale; f32 rotate; VEC2 translate; };
struct Transform3 { VEC3 scale, rotate, translate; Transform3(); Transform3(const VEC3&, const VEC3&, const VEC3&); };
template <typename T> T Min(T, T);
template <typename T> T Max(T, T);
template <typename T> T Clamp(T, T, T);
/* Independently expressed integral alignment; the mixed-type signature follows
 * public NW4C calls with unsigned sizes and integer literal alignments. */
template <typename T, typename A> inline T RoundUp(T value, A alignment) {
    return (value + static_cast<T>(alignment) - 1) & ~(static_cast<T>(alignment) - 1);
}
template <typename T, typename A> inline T RoundDown(T value, A alignment) {
    return value & ~(static_cast<T>(alignment) - 1);
}
f32 FAbs(f32);
f32 FSqrt(f32);
f32 FFloor(f32);
f32 FCeil(f32);
f32 FExp(f32);
f32 FLog(f32);
void SinCosFIdx(f32*, f32*, f32);
f32 SinFIdx(f32);
f32 CosFIdx(f32);
f32 ATan2FIdx(f32, f32);
VEC3* VEC3Normalize(VEC3*, const VEC3*);
VEC3* VEC3Cross(VEC3*, const VEC3*, const VEC3*);
f32 VEC3Dot(const VEC3*, const VEC3*);
VEC3* VEC3Sub(VEC3*, const VEC3*, const VEC3*);
VEC3* VEC3Add(VEC3*, const VEC3*, const VEC3*);
VEC3* VEC3Scale(VEC3*, const VEC3*, f32);
VEC3* VEC3Transform(VEC3*, const MTX34*, const VEC3*);
VEC3* VEC3TransformNormal(VEC3*, const MTX34*, const VEC3*);
MTX34* MTX34Copy(MTX34*, const MTX34*);
MTX34* MTX34Copy(MTX34*, const MTX34&);
MTX34* MTX34Zero(MTX34*);
MTX34* MTX34Identity(MTX34*);
MTX34* MTX34Mult(MTX34*, const MTX34*, const MTX34*);
MTX34* MTX34Inv(MTX34*, const MTX34*);
MTX34* MTX34Scale(MTX34*, const MTX34*, const VEC3*);
QUAT* MTX34ToQUAT(QUAT*, const MTX34*);
MTX34* QUATToMTX34(MTX34*, const QUAT*);
QUAT* QUATSlerp(QUAT*, const QUAT*, const QUAT*, f32);
/* Declaration-only signatures recovered from public gfx uses, not SDK code. */
f32 VEC3SquareLen(const VEC3*);
f32 VEC3Len(const VEC3*);
VEC3* VEC3Mult(VEC3*, const VEC3*, const VEC3*);
VEC3* VEC3Transform(VEC3*, const MTX33*, const VEC3*);
VEC3* VEC3TransformNormal(VEC3*, const MTX33*, const VEC3*);
MTX34* MTX34RotXYZRad(MTX34*, f32, f32, f32);
MTX34* MTX34Inverse(MTX34*, const MTX34*);
MTX34* MTX34Inverse(MTX34*, const MTX34&);
MTX34* MTX34InvTranspose(MTX34*, const MTX34*);
MTX34* MTX34Transpose(MTX34*, const MTX34*);
MTX33* MTX34ToMTX33(MTX33*, const MTX34*);
MTX34* MTX34MultScale(MTX34*, const MTX34*, const VEC3*);
MTX34* MTX34MultScale(MTX34*, const MTX34&, const VEC3&);
MTX34* MTX34MultTranslate(MTX34*, const MTX34*, const VEC3*);
MTX34* MTX33Mult(MTX34*, const MTX34*, const MTX34*);
MTX34* MTX33Mult(MTX34*, const MTX34*, const MTX34&);
MTX33* MTX33Mult(MTX33*, const MTX33&, const MTX34&);
MTX44* MTX44Copy(MTX44*, const MTX44*);
MTX34* MTX34LookAt(MTX34*, const VEC3*, const VEC3*, const VEC3*);
void SinCosRad(f32*, f32*, f32);
static const f32 PI = 3.14159265358979323846f;
namespace ARMv6 {}
} }
