class ServiceSingletonW22C {
public:
    ServiceSingletonW22C() {}
    /* INTEGRATOR: differs from src/wave3/empty_functions.c: func_001ebe1c is
     * used as this singleton's virtual destructor and receives its pointer. */
    virtual ~ServiceSingletonW22C();
};

extern "C" ServiceSingletonW22C *func_001ebda4(void)
{
    /* [INFERENCE] Singleton class identity is unrecovered. The empty destructor
     * registration and guard-release callees were inlined by armlink. */
    static ServiceSingletonW22C instance;
    return &instance;
}
