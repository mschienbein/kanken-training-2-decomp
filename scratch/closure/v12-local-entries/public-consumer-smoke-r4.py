"""Bounded successor smoke; retain every fdcc391 receipt without rewriting it."""
from contextlib import redirect_stdout
from copy import deepcopy
import io
import json
from pathlib import Path
import runpy
import sys

ROOT = Path.cwd()
sys.path.insert(0, str(ROOT))
from tools import analyze, catalogue_local_entries as local

# Reexercise the complete original bounded smoke against the fixed source. It
# checks the two private objects and consumes retained r4 metadata in memory;
# it neither discovers a current image nor writes a successor catalogue.
output = io.StringIO()
with redirect_stdout(output):
    previous = runpy.run_path(str(ROOT / "scratch/closure/v12-local-entries/public-consumer-smoke-r3.py"))
previous_result = json.loads(output.getvalue())


def require_extra_owner_refusal(candidate: dict, analyzer, evidence: dict,
                                 first: int, end: int, owner: int, kind: str) -> dict:
    changed = deepcopy(candidate)
    row = next(row for row in changed["ranges"] if (row["start"], row["end"]) == (first, end))
    assert row == {"start": first, "end": end, "kind": "unknown", "owner": None}
    row.update(kind=kind, owner=owner)
    assert changed["functions"] == candidate["functions"]
    assert changed["stats"] == candidate["stats"]
    assert {"start": owner, "end": owner + 10, "kind": "function", "owner": owner} in changed["ranges"]
    try:
        local.check_catalogue(changed, analyzer, evidence)
    except ValueError as error:
        assert "ownership outside its exact declared span" in str(error), str(error)
        return {"start": first, "end": end, "owner": owner, "kind": kind,
                "refused": True, "complete_records_and_statistics_unchanged": True}
    raise AssertionError("extra ownership outside the declared ten-byte entry was admitted")


data, synthetic = previous["data"], previous["synthetic"]
synthetic_analyzer = analyze.Analyzer(data["image"], data["layout"])
synthetic_positive = local.check_catalogue(synthetic, synthetic_analyzer, data["closure"])
assert local.baseline_hashes(synthetic_positive) == local.baseline_hashes(data["baseline"])
synthetic_refusals = [require_extra_owner_refusal(synthetic, synthetic_analyzer, data["closure"],
                                                first, end, 0x100020, kind)
                      for first, end in ((0x10004A, 0x10004C), (0x100050, 0x100060))
                      for kind in ("function", "unknown")]

retained, analyzer, evidence = previous["retained"], previous["analyzer"], previous["evidence"]
retained_positive = local.check_catalogue(retained, analyzer, evidence)
assert local.baseline_hashes(retained_positive) == evidence["local_entry_revision"]["baseline"]["field_sha256"]
retained_refusals = [require_extra_owner_refusal(retained, analyzer, evidence, first, end, owner, kind)
                     for first, end in ((0x1005EA, 0x1005EC), (0x1009FE, 0x100A00), (0x1005FA, 0x10060A))
                     for owner in (0x1005C0, 0x1009D4)
                     for kind in ("function", "unknown")]

private = previous["private"]
print(json.dumps({"status": "pass", "previous_bounded_smoke_reexercised": previous_result,
                  "synthetic_exact_span_positive_control": True, "synthetic_owner_injection_refusals": synthetic_refusals,
                  "retained_r4_exact_span_positive_control": True, "retained_owner_injection_refusals": retained_refusals,
                  "private_complete_section_proof": {"command": previous["private_command"],
                                                     "actual_exit_code": private.returncode,
                                                     "stdout": private.stdout, "stderr": private.stderr},
                  "new_C_credit": 0, "fresh_current_image_generation": False, "successor_catalogue_written": False},
                 indent=2, sort_keys=True))
