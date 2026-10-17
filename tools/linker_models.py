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
    function_entry_aliases: bool = False  # v5: proven same-section zero/full-sized entry aliases
    removed_unwind: bool = False  # v7: eliminate a removed helper's exclusive EHABI companions


LINKER_MODELS = {m.name: m for m in (
    LinkerModel("strict-v1"),
    LinkerModel("armlink5-inline-branchnop-v2", inline=True, branchnop=True),
    LinkerModel("armlink5-inline-branchnop-v3", inline=True, branchnop=True, removed_inline=True),
    LinkerModel("armlink5-inline-branchnop-v4", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True),
    LinkerModel("armlink5-inline-branchnop-v5", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True, function_entry_aliases=True),
    LinkerModel("armlink5-inline-branchnop-v7", inline=True, branchnop=True, removed_inline=True,
                bounded_switches=True, function_entry_aliases=True, removed_unwind=True),
)}
DEFAULT_LINKER_MODEL = "armlink5-inline-branchnop-v2"
# Names recorded by earlier artifacts whose rules are withdrawn: never run again (docs/LINKER.md).
RETIRED_LINKER_MODELS = {
    "armlink5-inline-branchnop-v1": "judged a callee defined in the candidate object by the candidate's own bytes, "
    "not the original's (review F01); superseded by armlink5-inline-branchnop-v2",
}

LITERAL_MODELS = ("none", "anonymous-string-records-v1")
DEFAULT_LITERAL_MODEL = "none"
