"""Heavy-slot-only: own, merged-Main, then throwaway-adopted regeneration."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT=Path(__file__).resolve().parents[3]
REPRO=ROOT/'scratch/repro/catalogue-v13-main'
OUT=ROOT/'scratch/closure/v13-alignment/generation'
OUT.mkdir(parents=True,exist_ok=True)
PYTHON='/home/michael/code/kanken-training-2-decomp/.venv/bin/python'
sha=lambda data:hashlib.sha256(data).hexdigest()
summary={'status':'running','main_commit':'6c9990ec2e863aead9953cc8adf9c0ed3aff7800',
         'canonical_adoption':False,'throwaway_root':str(REPRO),'steps':[]}

def save(path,value):
    path.write_text(json.dumps(value,indent=2,sort_keys=True)+'\n')

for name,cwd,path in [('branch',ROOT,ROOT/'scratch/analyze/catalogue-v13-r1.json'),
                      ('merged_main',REPRO,OUT/'merged-main-r1.json'),
                      ('throwaway_adopted',REPRO,OUT/'throwaway-adopted-r1.json')]:
    if name=='throwaway_adopted':
        target=REPRO/'config/catalogue.json'
        summary['throwaway_pre_adoption_sha256']=sha(target.read_bytes())
        proposal=(ROOT/'scratch/analyze/catalogue-v13-r1.json').read_bytes()
        target.write_bytes(proposal)
        summary['throwaway_adopted_sha256']=sha(target.read_bytes())
    argv=[PYTHON,'-B','tools/analyze.py','catalogue','--out',str(path)]
    start=time.perf_counter(); run=subprocess.run(argv,cwd=cwd,capture_output=True)
    receipt={'name':name,'argv':argv,'cwd':str(cwd),'exit_code':run.returncode,
             'seconds':time.perf_counter()-start,'stdout':run.stdout.decode(),
             'stderr':run.stderr.decode(),'output':str(path)}
    if not run.returncode:
        payload=path.read_bytes(); catalogue=json.loads(payload)
        receipt.update(proposal_sha256=sha(payload),proposal_bytes=len(payload),stats=catalogue['stats'],
                       remaining_unknown_ranges=[row for row in catalogue['ranges'] if row['kind']=='unknown'],
                       remaining_anomalies=catalogue['anomalies'],generator=catalogue['generator'])
    summary['steps'].append(receipt)
    save(OUT/f'{name}-receipt-r1.json',receipt)
    if run.returncode:
        summary['status']='failed_actual_generator_exit'
        save(OUT/'summary-r1.json',summary)
        print(json.dumps({'status':summary['status'],'step':name,'exit_code':run.returncode}))
        sys.exit(run.returncode)
    if name=='branch':
        expected=receipt['proposal_sha256']
    elif receipt['proposal_sha256']!=expected:
        summary['status']='failed_proposal_hash_disagreement'
        save(OUT/'summary-r1.json',summary)
        print(json.dumps({'status':summary['status'],'step':name,'exit_code':1}))
        sys.exit(1)
summary.update(status='pass',proposal_sha256=expected,all_three_proposal_hashes_equal=True)
save(OUT/'summary-r1.json',summary)
print(json.dumps({'status':'pass','proposal_sha256':expected,'actual_generator_exits':[row['exit_code'] for row in summary['steps']],
                  'all_three_proposal_hashes_equal':True,'canonical_adoption':False}))
