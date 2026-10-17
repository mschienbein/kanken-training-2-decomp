struct RuntimeInterface;
struct RuntimeInterfaces {
    struct RuntimeInterface *primary;
    struct RuntimeInterface *secondary;
};
extern struct RuntimeInterfaces data_00242f84;

void func_00116cd8(struct RuntimeInterface *interface)
{
    data_00242f84.secondary = interface;
}
