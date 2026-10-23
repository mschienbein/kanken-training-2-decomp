#include <preinclude.h>
#include "nw/lyt/lyt_Types.h"

/* [INFERENCE] The three-word layout and defaults agree with canonical
 * TevStage. The original application's class identity remains unproven. */
extern "C" nw::lyt::TevStage *func_0014b54c(nw::lyt::TevStage *stage)
{
    using namespace nw::lyt;
    stage->Set(TEVMODE_REPLACE, TEVMODE_REPLACE,
               TEVSRC_PRIMARY, TEVSRC_PRIMARY, TEVSRC_PRIMARY,
               TEVSRC_PRIMARY, TEVSRC_PRIMARY, TEVSRC_PRIMARY,
               TEVOPRGB_RGB, TEVOPRGB_RGB, TEVOPRGB_RGB,
               TEVOPALP_ALPHA, TEVOPALP_ALPHA, TEVOPALP_ALPHA,
               TEVSCALE_1, TEVSCALE_1, TEVKONSTSEL_K0, TEVKONSTSEL_K0,
               false, false);
    return stage;
}
