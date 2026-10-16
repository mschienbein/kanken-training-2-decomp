import itertools, sys
from pathlib import Path
sys.path.insert(0, 'tools')
import match, toolchain
ctx = match.Context(Path('config/catalogue.json'))
stmts = ["e->a = a;", "e->z = 0;", "e->x = (float)x;", "e->y = (float)y;"]
head = """struct E1091d4 { char pad0[0x1c]; int a; char pad1[0x28 - 0x20]; float x; char pad2[4]; float y; int z; char pad3[0x3c - 0x38]; };
extern struct E1091d4 bss_0049c118[];
void func_001091d4(int idx, int a, int x, int y) {
    struct E1091d4 *e = &bss_0049c118[idx];
"""
base = ["--cpu=MPCore", "--fpmode=fast", "--apcs=/interwork", "--arm", "--split_sections", "--signed_chars", "-O3", "-Otime"]
for cid in sys.argv[1:]:
    ok = []
    for perm in itertools.permutations(range(4)):
        src = Path(f'scratch/compid/perm/p_{"".join(map(str,perm))}.c'); src.parent.mkdir(parents=True, exist_ok=True)
        src.write_text(head + "\n".join("    " + stmts[i] for i in perm) + "\n}\n")
        obj = src.with_suffix(f'.{cid}.o')
        toolchain.compile_source(toolchain.Profile('p', cid, tuple(base)), src.resolve(), obj)
        r = ctx.compare(obj)[0]
        if r['status'] == 'match': ok.append(perm)
    print(cid, 'matching orders:', ok)
