#include "game/service_lifetime.h"
#include "ctr/svc.h"

/* [INFERENCE] Result comparison is performed on a one-word SDK value object. */
class SdkResult {
    CtrResult value;
public:
    SdkResult(CtrResult result) : value(result) {}
    bool operator==(const SdkResult &other) const { return value == other.value; }
    operator CtrResult() const { return value; }
};


extern "C" CtrResult func_0012d3e4(void)
{
    SdkResult result = func_00136278();
    if (result == SdkResult((CtrResult)0xe1209c46u))
        return result;
    func_0012d478();
    return result;
}
