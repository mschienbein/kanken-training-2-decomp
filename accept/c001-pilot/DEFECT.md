# c001-pilot: defective prepare record — never finalized

`prepare.json` was written by a tools/accept.py that omitted the `checks` field (an editing error
removed `"checks": checks` from the record). The computed `result` (PASS) and every recorded
input are intact, but the record does not show the per-gate results, so it is not reviewed or
finalized. The same candidates were prepared again as `c001-pilot-r2` after the fix
(docs/GATES.md v1.1). This directory is kept unchanged as history.
