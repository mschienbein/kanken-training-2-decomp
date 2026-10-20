#line 1 "/home/michael/code/kanken-training-2-decomp/scratch/permute/sweep-c1/inputs/func_001efcd0.c"

 
struct WordMember08 {
    unsigned char reserved00[8];
    unsigned int value;
};
struct ObjectMember68 {
    unsigned char reserved00[0x68];
    const struct WordMember08 *member;
};

extern unsigned int func_001efcd8(const struct WordMember08 *member);

unsigned int func_001efcd0(const struct ObjectMember68 *object)
{
    return func_001efcd8(object->member);
}
