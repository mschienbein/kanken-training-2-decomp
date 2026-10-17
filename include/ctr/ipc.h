#ifndef CTR_IPC_H
#define CTR_IPC_H

#include <string.h>
#include "ctr/cp15.h"
#include "ctr/svc.h"

typedef union ByteCommandWord {
    unsigned int word;
    CtrResult result;
    unsigned short halfword;
    unsigned char byte;
} CtrIpcWord;

struct IdentifierCommand {
    unsigned int header;
    union {
        unsigned short identifier;
        CtrResult result;
    } word;
};

struct ByteSelectorCommand {
    unsigned int header;
    union {
        unsigned char selector;
        CtrResult result;
    } status;
};

typedef struct CtrIpcStaticBuffer {
    unsigned int descriptor;
    void *buffer;
} CtrIpcStaticBuffer;

typedef union CtrIpcWideValue {
    unsigned long long value;
    unsigned int words[2];
} CtrIpcWideValue;

/* CTR command and static-buffer areas belong to the current thread. */
static __forceinline unsigned int *ctrIpcCommandBuffer(void)
{
    return (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
}

static __forceinline unsigned int *ctrIpcStaticBufferWords(void)
{
    return (unsigned int *)(ctrGetThreadLocalStorage() + 0x180);
}

static __forceinline unsigned int ctrIpcHeader(unsigned int command,
                                             unsigned int normal_words,
                                             unsigned int translated_words)
{
    return (command << 16) | (normal_words << 6) | translated_words;
}

static __inline unsigned int ctrIpcStaticBufferDescriptor(unsigned int size,
                                                         unsigned int index)
{
    unsigned int descriptor = index << 10;
    descriptor |= size << 14;
    return descriptor | 2u;
}

/* permission: 2 = readable, 4 = writable, 6 = readable and writable. */
#define ctrIpcMappedBufferDescriptor(size, permission) \
    (((size) << 4) | 8u | (permission))

static __forceinline int ctrIpcTransportFailure(CtrResult result)
{
    return (int)((unsigned int)result & 0x80000000u);
}

/* [INFERENCE] Address-taken, width-preserving marshalling of a narrow
 * parameter's representation. The unaddressed bytes of its IPC word remain
 * uninitialized. This union view and the halfword macro also constrain ARMCC's
 * parameter homing; their source-shape qualification is recorded for c007. */
union CtrIpcByteParameter {
    unsigned char value;
    unsigned char bytes[1];
};

static __forceinline void ctrIpcMarshalByte(unsigned char *destination,
                                          const unsigned char *source)
{
    *destination = ((const union CtrIpcByteParameter *)source)->value;
}

#define ctrIpcMarshalHalfword(destination, source) \
    memcpy((destination), (source), sizeof(*(source)))

#ifdef __cplusplus
template <typename T>
static inline void ctrIpcMarshal(T &destination, const T &source)
{
    memcpy(&destination, &source, sizeof(T));
}

template <>
inline void ctrIpcMarshal<unsigned char>(unsigned char &destination,
                                        const unsigned char &source)
{
    ctrIpcMarshalByte(&destination, &source);
}

/* Address-taken, width-preserving IPC marshalling. ARMCC 4.1 retains the
 * parameter's real byte/halfword storage; no volatile spill steering is used. */
static inline void ctrMarshalValue(unsigned char &output, const unsigned char &input)
{
    output = input;
}

static inline void ctrMarshalValue(unsigned short &output, const unsigned short &input)
{
    output = input;
}

extern "C" {
#endif
extern CtrHandle data_00243040;
extern CtrResult func_0010d418(unsigned char value);
extern CtrResult func_00115a98(unsigned char value);
extern CtrResult func_001235f8(unsigned char selector);
extern CtrResult func_00148118(const CtrHandle *session, unsigned short identifier);
extern CtrResult func_0014aed0(const CtrHandle *session, unsigned short identifier);
#ifdef __cplusplus
}
#endif

#endif
