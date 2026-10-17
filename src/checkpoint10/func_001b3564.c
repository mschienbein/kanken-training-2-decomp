struct ChoiceController;
extern void func_001b38c4(struct ChoiceController *controller, int index);

void func_001b3564(struct ChoiceController *controller)
{
    int index;
    for (index = 0; index < 23; ++index)
        func_001b38c4(controller, index);
}
