"""Consume sealed v12 metadata through v13; not a catalogue discovery/generation."""
from copy import deepcopy
import hashlib
import json
from pathlib import Path
import sys
ROOT=Path(__file__).resolve().parents[3]
sys.path[:0]=[str(ROOT/'tools'),str(ROOT)]
from tools import analyze, catalogue_alignment as alignment, catalogue_closure, ctrfmt
path=ROOT.parents[2]/'scratch/review/catalogue-v12-r2/codex/proposal.json'
raw=path.read_bytes()
assert hashlib.sha256(raw).hexdigest()=='ee64b7d69bf5b96fe26a8ada241a59eca2924ac4ab9a376a5fc449e2d1e5a942'
baseline=json.loads(raw)
image=(ROOT/'orig/code.bin').read_bytes()
layout=ctrfmt.parse_exheader((ROOT/'orig/exheader.bin').read_bytes())
analyzer=analyze.Analyzer(image,layout)
evidence,_,_=catalogue_closure.load_inputs(ROOT,ROOT/'research/catalogue_v9_r2_evidence.json')
candidate=deepcopy(baseline)
alignment.close_catalogue(candidate,analyzer,evidence,{})
restored=alignment.check_catalogue(candidate,analyzer,evidence)
assert alignment.baseline_hashes(restored)==alignment.baseline_hashes(baseline)
assert candidate['stats']['bytes_by_range_kind']['unknown']==114
assert candidate['stats']['unknown_ranges']==11
assert candidate['anomalies']==[{'address':0x10d2b8,'issue':'unexplained_function_bytes'}]
assert candidate['stats']['creditable_functions']==4571
assert candidate['stats']['creditable_bytes']==1121674
assert len(candidate['functions'])==4743
assert sum(len(fn['entries']) for fn in candidate['functions'])==4776
print(json.dumps({'status':'pass','current_image_generation':False,'adoption':False,
                  'baseline_sha256':hashlib.sha256(raw).hexdigest(),'stats':candidate['stats'],
                  'transitions':evidence['alignment_revision']['transitions'],
                  'non_c_transitions':evidence['alignment_revision']['_non_c_proof']['classifications'],
                  'remaining_unknown_ranges':[row for row in candidate['ranges'] if row['kind']=='unknown'],
                  'remaining_anomalies':candidate['anomalies']},indent=2,sort_keys=True))
