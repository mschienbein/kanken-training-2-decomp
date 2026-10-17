#ifndef CTR_IPC_H
#define CTR_IPC_H

#include "ctr/cp15.h"
#include "ctr/svc.h"

union ByteCommandWord {
    unsigned int word;
    unsigned char byte;
};

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

#ifdef __cplusplus
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
