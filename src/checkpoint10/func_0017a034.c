/* INTEGRATOR: share the numeric-display types in include/game/number_display.h. */
struct W15gNumberFormat {
    /* [INFERENCE] First word selects the renderer: 0/6, 1, 2-5 (callee func_001ea314 reads it as the font). */
    unsigned kind;
    unsigned reserved04[6];
};
struct W15gNumberGroup;
struct W15gNumberController {
    unsigned char reserved00[0xc];
    struct W15gNumberFormat formats[72];
    struct W15gNumberGroup *groups[];
};

/* INTEGRATOR: differs from src/wave4/func_00166f1c.c: the 0x7ec group-pointer array proves a numeric-controller pointer instead of void *. */
extern void func_00166f20(struct W15gNumberController *controller, int index, int enabled);
extern void func_001ea314(struct W15gNumberController *controller, int index, const struct W15gNumberFormat *format, int value);
extern void func_001ea1cc(struct W15gNumberController *controller, int index, const struct W15gNumberFormat *format, int value);
extern void func_001ea0cc(struct W15gNumberController *controller, int index, const struct W15gNumberFormat *format, int value);

static void draw_number(struct W15gNumberController *controller, int index, int value)
{
    struct W15gNumberGroup *group = controller->groups[index];
    if (group) {
        const struct W15gNumberFormat *format = &controller->formats[index];
        switch (format->kind) {
        case 0: case 6:
            func_001ea314(controller, index, format, value);
            break;
        case 1:
            func_001ea1cc(controller, index, format, value);
            break;
        case 2: case 3: case 4: case 5:
            func_001ea0cc(controller, index, format, value);
            break;
        }
    }
}

void func_0017a034(struct W15gNumberController *controller, int index, int value)
{
    func_00166f20(controller, index, 0);
    draw_number(controller, index, value);
    func_00166f20(controller, index, 1);
}
