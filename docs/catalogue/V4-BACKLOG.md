# Catalogue v4 backlog

Findings against the adopted catalogue (v3, `docs/catalogue/REPORT-v3.md`). Fix them together in
one new catalogue version, adopted between checkpoints via `tools/denominator.py record`.

1. **Pointer-referenced NOP padding.** 0x186eec (`mov r0, r0`) is classed `padding`, but
   func_0019bc7c loads 0x186eec as a callback address (0x19bf20–28), and the NOP falls into
   0x186ef0. Pointer evidence must create an entry. (Wave2Author3, 2026-10-16.)
2. **NOP entries should not make the following function non-C.** v3 groups each 4-byte NOP entry
   with the function it falls into as one `multi_entry` routine, so the following function (21 v3
   groups plus item 1) is not creditable even when its body is ordinary C. Proposed model: the
   NOP stays a separate 4-byte non-creditable record (`kind: "nop_entry"`, its address a valid
   call target), and the following function remains `kind: "function"`. Before adopting this,
   find out what emits these NOPs (same-body alias with distinct addresses? SDK/linker
   convention?) and check that a C body can match at the following start.
3. **Trailing `mov r0, r0` before `bx lr`** in some functions (e.g. 0x137118, 0x125b50): same
   NOP idiom at function ends; check whether the compiler or a source construct produces it.
