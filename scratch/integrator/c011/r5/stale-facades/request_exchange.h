#ifndef GAME_REQUEST_EXCHANGE_H
#define GAME_REQUEST_EXCHANGE_H

/* Caller/initializer-owned record shared by00145840 and0019051c. The receive
 * operation's separate five-versus-six-input ABI remains unresolved; this
 * record definition does not endorse RequestWait or a receive-call adaptor. */
struct RequestRecord {
    unsigned kind;
    unsigned flags;
    unsigned short count;
    unsigned short length;
    unsigned short text[0x76c];
    unsigned char flag_ee4;
    unsigned char flag_ee5;
    unsigned char flag_ee6;
    unsigned char auxiliary[0x89];
    int handle;
    unsigned short status;
    unsigned char reservedf76[10];
};

#ifdef __cplusplus
extern "C" {
#endif
void func_0019051c(struct RequestRecord *record);
void func_00190450(unsigned char *output, struct RequestRecord *record);
#ifdef __cplusplus
}
#endif

#endif
