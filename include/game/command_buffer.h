#ifndef GAME_COMMAND_BUFFER_H
#define GAME_COMMAND_BUFFER_H



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
class W19CommandBuffer;

class W19CommandBuffer {
public:
    virtual void reserved00();
    virtual void reserved01();
    virtual void reserved02();
    virtual void reserved03();
    virtual unsigned char *commit(unsigned char *cursor, unsigned size);
    virtual void reserved05();
    virtual unsigned char *reserve(unsigned char *cursor, unsigned size);
};

extern "C" {
extern unsigned char *data_00242fc8;
}
#endif

#endif
