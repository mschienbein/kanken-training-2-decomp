"""Linker and literal model registry (docs/LINKER.md), free of disassembler dependencies.

`tools/inplace.py` implements the models; `tools/accept.py` only needs their names to record and
verify checkpoint selections, and `accept.py verify` must run in CI without capstone.
"""

from dataclasses import dataclass


@dataclass(frozen=True)
class LinkerModel:
    """The armlink output optimisations an in-place link reproduces.

    The name is the version: a rule change is a new model, never an edited one, so the model name
    recorded in match results and build receipts says exactly which rules credited a function.
    """

    name: str
    inline: bool = False  # --inline: ARM BL to a 4/8-byte ARM function -> its instruction, or a NOP
    branchnop: bool = False  # --branchnop: relocated ARM B to the next instruction -> NOP
    removed_inline: bool = False  # v3: inline and eliminate candidate-only tiny function sections
    bounded_switches: bool = False  # v4: bounded ARM jump-table CFG with fail-closed endian state
    function_entry_aliases: bool = False  # v6: proven aliases plus independently bound legacy placement
    removed_unwind: bool = False  # v8: eliminate only complete, exclusive EHABI/group closures
    unreferenced_destructors: bool = False  # v9: witnessed, unbound D0 sections with no live references
    unreferenced_destructor_unwind: bool = False  # v10: U1 owners receive v8's exclusive own-unwind closure
    checked_destructor_removal: bool = False  # v11: complete allocated census, exact mappings/target identities
    complete_section_metadata: bool = False  # v12: unreduced ELF header and group/copy identity
    structural_companion_edges: bool = False  # v13: exempt exact own structural edges, never whole sources
    complete_symbol_resolution: bool = False  # v14: all supplied definitions, scoped refs and ABI-marker namespace


LINKER_MODELS = {m.name: m for m in (
    LinkerModel("strict-v1"),
    LinkerModel("armlink5-inline-branchnop-v2", inline=True, branchnop=True),
    LinkerModel("armlink5-inline-branchnop-v3", inline=True, branchnop=True, removed_inline=True),
    LinkerModel("armlink5-inline-branchnop-v4", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True),
    LinkerModel("armlink5-inline-branchnop-v6", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True, function_entry_aliases=True),
    LinkerModel("armlink5-inline-branchnop-v8", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True, function_entry_aliases=True, removed_unwind=True),
    LinkerModel("armlink5-inline-branchnop-v9", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True, function_entry_aliases=True, removed_unwind=True,
                unreferenced_destructors=True),
    LinkerModel("armlink5-inline-branchnop-v10", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True, function_entry_aliases=True, removed_unwind=True,
                unreferenced_destructors=True, unreferenced_destructor_unwind=True),
    LinkerModel("armlink5-inline-branchnop-v11", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True, function_entry_aliases=True, removed_unwind=True,
                unreferenced_destructors=True, unreferenced_destructor_unwind=True,
                checked_destructor_removal=True),
    LinkerModel("armlink5-inline-branchnop-v12", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True, function_entry_aliases=True, removed_unwind=True,
                unreferenced_destructors=True, unreferenced_destructor_unwind=True,
                checked_destructor_removal=True, complete_section_metadata=True),
    LinkerModel("armlink5-inline-branchnop-v13", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True, function_entry_aliases=True, removed_unwind=True,
                unreferenced_destructors=True, unreferenced_destructor_unwind=True,
                checked_destructor_removal=True, complete_section_metadata=True,
                structural_companion_edges=True),
    LinkerModel("armlink5-inline-branchnop-v14", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True, function_entry_aliases=True, removed_unwind=True,
                unreferenced_destructors=True, unreferenced_destructor_unwind=True,
                checked_destructor_removal=True, complete_section_metadata=True,
                structural_companion_edges=True, complete_symbol_resolution=True),
)}
DEFAULT_LINKER_MODEL = "armlink5-inline-branchnop-v2"
# Names recorded by earlier artifacts whose rules are withdrawn: never run again (docs/LINKER.md).
RETIRED_LINKER_MODELS = {
    "armlink5-inline-branchnop-v1": "judged a callee defined in the candidate object by the candidate's own bytes, "
    "not the original's (review F01); superseded by armlink5-inline-branchnop-v2",
    "armlink5-inline-branchnop-v5": "reviewed 22cb05b refuses explicitly bound D1/D2 placements that v4 credits "
    "(func_001c6a68, func_001ef89c); superseded by v6, never adopted",
    "armlink5-inline-branchnop-v7": "permits partial removal of live COMDAT groups and accepts compact extab "
    "entries without their descriptor-list terminator (exidx-v7 review F01/F02); superseded by v8, never adopted",
}

LITERAL_MODELS = ("none", "anonymous-string-records-v1")
DEFAULT_LITERAL_MODEL = "none"
