/* [INFERENCE] Callback role: func_00177b10 registers func_001b7dac,
 * which reads global state and returns an integer without reading inputs. */
struct RegisteredCallback {
    unsigned reserved00[4];
    unsigned state;
    int (*callback)(void);
};

extern struct RegisteredCallback data_0023ee9c;

void func_0015dec4(int (*callback)(void))
{
    data_0023ee9c.state = 0;
    data_0023ee9c.callback = callback;
}
