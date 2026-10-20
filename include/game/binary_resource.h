#ifndef GAME_BINARY_RESOURCE_H
#define GAME_BINARY_RESOURCE_H
struct BinaryResourceHeader { int type; };
struct BinaryResourceReference { struct BinaryResourceHeader *resource; };
/* Serialized offset is relative to its own word, not to the owner base. */
struct RelativeBinaryResource {
    unsigned char reserved00[8];
    int offset;
    unsigned char reserved0c[0x20];
    unsigned state;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_001840dc(struct BinaryResourceReference *reference);
#ifdef __cplusplus
}
#endif
#endif
