#ifndef GAME_TIMED_SCREEN_H
#define GAME_TIMED_SCREEN_H

struct W15gTimedScreenVTable;
struct W15gTimedScreen;
struct W15gNumberController;



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */


struct W15gTimedScreenVTable {
    unsigned char reserved00[0x18];
    void (*update)(W15gTimedScreen *screen);
    unsigned reserved1c;
    void (*finish)(W15gTimedScreen *screen);
};
struct W15gTimedScreen {
    const W15gTimedScreenVTable *vtable;
    unsigned char reserved04[0x44];
    W15gNumberController *numbers;
    unsigned char reserved4c[0x108];
    unsigned char active;
    unsigned char reserved155[0x27];
    unsigned delay;
};

extern "C" {


void func_001c1590(unsigned ignored);
void func_001dbf40(unsigned *value, unsigned *minutes, unsigned *seconds, unsigned *milliseconds);
void func_0017a034(W15gNumberController *controller, int index, int value);
void func_001ef704(W15gTimedScreen *screen);
}
#endif

#endif
