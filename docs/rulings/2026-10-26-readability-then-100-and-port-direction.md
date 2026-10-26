# Ruling 2026-10-26: readability phase, then 100% matching; port later in Rust, Windows/Surface first

**Decision (user, 2026-10-26).**

1. **Readability now.** Start the readability phase that docs/ACCEPTANCE.md (lines 198–201) counts toward done, and model it on the Shiren decomp's readability work:
   - evidence-backed names for functions, data, types and files, with real SDK/library names for library code;
   - subsystem folders;
   - semantic headers, enums and fields;
   - subsystem docs.

   Every round keeps `code.bin` byte-identical and runs through the ordinary checkpoint gates. Shiren is a read-only reference.
2. **Progress stays visible.** ByteMatch must keep showing progress; it ingests the report from every `main` push. README.md must reflect current progress: matching, and readability metrics once they exist.
3. **Then 100% matched.** After readability, drive matching toward 100%. Re-attack the residue with the owner identities and types that readability establishes. When only rule-blocked functions remain, they go back to the user. This supersedes nothing yet: the 2026-10-25 residue rulings stand until new evidence or a new ruling reopens a function.
4. **Port later.**
   - **Language and target:** Rust; Windows first, targeting a Microsoft Surface tablet so the touchscreen can be used.
   - **Model:** the owner's ToeJam & Earl port is the reference, read-only.

   Planning starts after the decomp phases; this ruling only records the direction. It refines the port plan in ACCEPTANCE.md's P level (Windows and iPad input).
5. **Concurrency:** at most **8** concurrent subagents, nested ones included (previously 16).
