#ifndef GAME_RESULT_H
#define GAME_RESULT_H

/* One-word result code (CTR-SDK style: bit 31 set means failure). Shared by every C++ unit so
 * the class has a single definition. */
struct Result {
    unsigned value;
#ifdef __cplusplus
    Result(unsigned code = 0) : value(code) {}
    unsigned failed() const { return value >> 31; }
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] Possible erased failure-bit accessor, not a recovered historical identity. */
unsigned result_failure_bits(struct Result result);
#ifdef __cplusplus
}
#endif




#endif
