# Ruling 2026-10-25: extend the linker model to Thumb branch-to-NOP

**Decision (user, 2026-10-25).** Add Thumb return-only inlining (armlink replacing a Thumb `BL` to a return-only Thumb callee with NOP-equivalent instructions) to the emulated linker model. Do it through a reviewed gate revision (GATES.md revision, implementation, independent implementation review and exact-text review), not by lowering any admission standard.

**Context.** The ARM Compiler 4.1 runtime libraries (builds 791 and 894; Arm ACOMP5, MD5-verified; provenance under ~/code/3ds-experiments/inputs/armcc-4.1-b*/) prove 3 of the 9 Thumb library candidates at discovery level. Five more (0010080a mbrlen, 00100914 _printf_cs_common, 00100af8 _printf_lcs_common, 0018fd68 mbstowcs, 0019fe04 mbrtowc; 276 B) are byte-identical to library members except one call slot each, where the image has NOP-equivalent instructions (scratch/libid/thumb-near-miss-REPORT.md). docs/LINKER.md currently states that Thumb inlining is not modelled because no small return-only Thumb function existed. Catalogue revisions since then introduced one: the 2-byte Thumb owner func_00100c9c. [INFERENCE] The NOP slots are armlink inlining calls to return-only Thumb stubs.

**Conditions.**
- The new model follows armlink's documented rule (Table 4-1, "Factors that influence function inlining"): Thumb → Thumb callees of 2 to 6 bytes ending in a return; a callee consisting only of a return sequence inlines as a NOP. It applies only where the callee's identity and extent are proven, in the image or the linked archive member.
- Library identities that rely on the new model are classified only after the gate revision is adopted, through a reviewed catalogue revision that cites the model.
- The 6-byte shared-entry bridge at 0x0019f9a4 matches no library family. It stays in the residue table unless new evidence appears.
