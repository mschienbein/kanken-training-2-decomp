#include "game/functions.h"

/* [INFERENCE] Decoder storage prefix: only the first-word external dispatch
 * address is recovered. This is not a native virtual/destructor identity,
 * a callable table declaration or a complete original table extent. */
struct TextDecoderDispatch;
extern "C" const TextDecoderDispatch data_00224cc0;
struct TextDecoderBase {
    const TextDecoderDispatch *dispatch;
    TextDecoderBase();
};

TextDecoderBase::TextDecoderBase() : dispatch(&data_00224cc0)
{
    /* [INFERENCE] callee identity: inlined by armlink. The established empty
     * diagnostic callback has the same two-unsigned-argument call contract. */
    func_0014f7e8(0, 0x04000085);
}
