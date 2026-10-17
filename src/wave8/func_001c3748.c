struct TransformTarget;

/* [INFERENCE] The caller supplies a target group and discards the live counter;
 * the source-level return contract is unresolved. Codegen qualification: this
 * empty counted loop plus its counter result retains the observed loop shape.
 * Neither the shorter constant return nor a void empty loop reproduces it. */
int func_001c3748(struct TransformTarget *const *objects)
{
    int i;
    for (i = 0; i < 5; ++i) {
    }
    return i;
}
