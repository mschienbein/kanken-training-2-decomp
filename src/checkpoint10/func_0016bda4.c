struct W24BPositionPanel;

extern void func_001c3d14(struct W24BPositionPanel *panel, int index,
                          int selection, int detail);
extern void func_0016bde0(struct W24BPositionPanel *panel, int index,
                          int mode, float x, float y);

void func_0016bda4(struct W24BPositionPanel *panel, int index,
                   int selection, int detail, float x, float y)
{
    func_001c3d14(panel, index, selection, detail);
    func_0016bde0(panel, index, 1, x, y);
}
