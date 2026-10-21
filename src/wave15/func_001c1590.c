/* 001edbb0 stores the 001c10f4 frame result at scene +0x17c; 001ef704
 * forwards that same frame, without the settling loop consuming it.
 * The function does not consume that value, and the caller ignores the result. */
struct L044TrainingFrame;
void func_001c1590(struct L044TrainingFrame *ignored)
{
    int i;
    for (i = 0; i < 27; ++i) {
    }
}
