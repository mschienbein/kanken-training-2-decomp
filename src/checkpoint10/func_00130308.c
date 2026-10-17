#include "ctr/svc.h"
struct ServiceFallbackState {
    unsigned char identifier;
    unsigned char enabled;
};
extern struct ServiceFallbackState data_00243044;
extern CtrResult func_001383ac(unsigned char identifier);
extern CtrResult func_00138334(unsigned char identifier);

CtrResult func_00130308(void)
{
    CtrResult result = data_00243044.enabled;
    if (result) {
        result = func_001383ac(data_00243044.identifier);
        if ((unsigned int)result == 0xc9405001u)
            return func_00138334(data_00243044.identifier);
    }
    return result;
}
