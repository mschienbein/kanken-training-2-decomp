"""Metadata-only adjudication requested by Main; no generation/build/ledger audit."""
import hashlib
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[3]
MAIN=ROOT.parents[2]
sha=lambda data:hashlib.sha256(data).hexdigest()
canon=lambda value:(json.dumps(value,indent=2,sort_keys=True,ensure_ascii=False)+'\n').encode()
proposal_path=ROOT/'scratch/analyze/catalogue-v13-r1.json'
payload=proposal_path.read_bytes(); proposal=json.loads(payload)
assert sha(payload)=='e333779b6f2272c70cc166f915aa925abbcb06641a11f62f4443368ab847e743'
v12path=MAIN/'scratch/review/catalogue-v12-r2/codex/proposal.json'
v12raw=v12path.read_bytes(); v12=json.loads(v12raw)
assert sha(v12raw)=='ee64b7d69bf5b96fe26a8ada241a59eca2924ac4ab9a376a5fc449e2d1e5a942'
v9path=ROOT/'research/catalogue_v13_frozen/config/catalogue.json'
v9raw=v9path.read_bytes(); v9=json.loads(v9raw)
assert sha(v9raw)=='d13a8c794676c135b3bcd03676bf1803c5b5215455428ee4177ed911311c3d64'
functions={row['address']:row for row in proposal['functions']}
nonc=json.loads((ROOT/'research/catalogue_v13_non_c_proof.json').read_bytes())
revision=json.loads((ROOT/'research/catalogue_v13_evidence.json').read_bytes())


def metrics(catalogue):
    ordinary=[row for row in catalogue['functions'] if row['kind']=='function']
    return {'owners':len(catalogue['functions']),'entries':sum(len(row['entries']) for row in catalogue['functions']),
            'ordinary_functions':len(ordinary),'creditable_functions':catalogue['stats']['creditable_functions'],
            'ordinary_extent_bytes':sum(row['size'] for row in ordinary),'creditable_bytes':catalogue['stats']['creditable_bytes'],
            'physical_function_bytes':catalogue['stats']['bytes_by_range_kind']['function'],
            'code_bytes':catalogue['stats']['code_bytes'],'pool_bytes':catalogue['stats']['pool_bytes'],
            'literal_bytes':catalogue['stats']['literal_bytes'],'padding_bytes':catalogue['stats']['bytes_by_range_kind']['padding'],
            'rodata_bytes':catalogue['stats']['bytes_by_range_kind']['rodata'],
            'unknown_bytes':catalogue['stats']['bytes_by_range_kind']['unknown'],
            'unknown_intervals':catalogue['stats']['unknown_ranges'],'anomalies':len(catalogue['anomalies'])}


def certificate(function):
    if 'classification_evidence' in function:
        return {'type':'classification_evidence','sha256':sha(canon(function['classification_evidence'])),
                'record':function['classification_evidence']}
    if function['kind']=='multi_entry':
        found=[row for row in proposal['closure_evidence']['owner_merges'] if row['start']==function['address']]
        if found:return {'type':'topology_not_non_C','record':found[0]}
    return None


def compare(name,baseline,path,raw):
    old={row['address']:row for row in baseline['functions']}
    before,after=metrics(baseline),metrics(proposal)
    changes=[]
    for address in sorted(old.keys()&functions.keys()):
        left,right=old[address],functions[address]
        if left!=right:
            changes.append({'address':address,'address_hex':f'{address:08x}',
                            'before':{key:left[key] for key in ('size','isa','kind')},
                            'after':{key:right[key] for key in ('size','isa','kind')},
                            'changed_fields':sorted(key for key in left.keys()|right.keys() if left.get(key)!=right.get(key)),
                            'before_sha256':sha(canon(left)),'after_sha256':sha(canon(right)),
                            'classification_certificate':certificate(right) if left['kind']!=right['kind'] else None})
    added=[{'address':address,'size':functions[address]['size'],'isa':functions[address]['isa'],'kind':functions[address]['kind'],
            'certificate':certificate(functions[address]),'entry_evidence':functions[address]['evidence']}
           for address in sorted(functions.keys()-old.keys())]
    removed=[{'address':address,'size':old[address]['size'],'isa':old[address]['isa'],'kind':old[address]['kind'],
              'retained_as_secondary_entry':any(entry['address']==address and entry['isa']==old[address]['isa']
                                               for fn in functions.values() for entry in fn['entries']),
              'replacement_certificate':next((row for row in proposal['closure_evidence']['owner_merges'] if address in row['legacy_owners']),None)}
             for address in sorted(old.keys()-functions.keys())]
    classification_changes=[row for row in changes if row['before']['kind']!=row['after']['kind']]
    assert all(row['classification_certificate'] for row in classification_changes)
    old_entries={(entry['address'],entry['isa']) for fn in old.values() for entry in fn['entries']}
    new_entries={(entry['address'],entry['isa']) for fn in functions.values() for entry in fn['entries']}
    return {'baseline':{'path':str(path),'sha256':sha(raw)},'before':before,'after':after,
            'delta':{key:after[key]-before[key] for key in before},'complete_record_changes':changes,
            'classification_changes':classification_changes,'added_owners':added,'removed_owners':removed,
            'retained_old_entry_identities':len(old_entries&new_entries),'lost_old_entries':sorted(old_entries-new_entries),
            'new_entries':sorted(new_entries-old_entries)}

comparisons={'v12_r2':compare('v12_r2',v12,v12path,v12raw),'v9_r2':compare('v9_r2',v9,v9path,v9raw)}
new_addresses={row['address'] for row in nonc['classifications']}
assert {row['address'] for row in comparisons['v12_r2']['complete_record_changes']}==new_addresses
assert not comparisons['v12_r2']['added_owners'] and not comparisons['v12_r2']['removed_owners']
assert not comparisons['v12_r2']['lost_old_entries'] and not comparisons['v9_r2']['lost_old_entries']
for row in comparisons['v12_r2']['complete_record_changes']:
    assert row['changed_fields']==['classification_evidence','kind']

# Only a selected-address intersection of metadata manifests. No source/ABI,
# object, compile, match, compatibility or whole-ledger acceptance audit follows.
paths=[MAIN/'config/matches.json',MAIN/'accept/c009-r3/matches.json',
       MAIN/'scratch/worktrees/c009-r4/config/candidates/c009.json',
       MAIN/'scratch/worktrees/c010-g3-unify/config/candidates/c010.json',
       MAIN/'scratch/worktrees/c011-r3-closure/config/candidates/c011.json',
       MAIN/'scratch/worktrees/cmodels2-r3-closure/config/candidates/cmodels2.json',
       MAIN/'scratch/worktrees/c012/config/candidates/c012.json']
old_differences={row['address'] for row in comparisons['v9_r2']['complete_record_changes']}
padding_owners={row['owner'] for row in revision['transitions'] if row['owner'] is not None}
intersections=[]
for path in paths:
    content=path.read_bytes(); ledger=json.loads(content)
    if path.parts[-3]=='config':
        base=path.parents[2]
    elif 'worktrees' in path.parts:
        base=path.parents[2]
    else:
        base=MAIN
    names_path=base/'config/names.json'
    names_raw=names_path.read_bytes(); names=json.loads(names_raw)['symbols']
    hits=[];claims=0
    for unit in ledger['units']:
        for name in unit['functions']:
            claims+=1
            address=(names[name]['address'] if name in names else int(name.removeprefix('func_'),16))&~1
            if address in new_addresses|old_differences|padding_owners:
                hits.append({'name':name,'address':address,'source':unit['source'],'profile':unit['profile'],
                             'new_v13_complete_function_record_change':address in new_addresses,
                             'inherited_since_v9_complete_record_change':address in old_differences-new_addresses,
                             'v13_padding_association':address in padding_owners})
    intersections.append({'manifest':str(path.relative_to(MAIN)),'manifest_sha256':sha(content),'claim_count':claims,
                          'names':str(names_path.relative_to(MAIN)),'names_sha256':sha(names_raw),
                          'new_v13_function_record_hits':[row for row in hits if row['new_v13_complete_function_record_change']],
                          'inherited_v9_record_hits':[row for row in hits if row['inherited_since_v9_complete_record_change']],
                          'padding_association_hits':[row for row in hits if row['v13_padding_association']]})

result={'status':'pass_metadata_only','proposal':{'path':str(proposal_path.relative_to(ROOT)),'sha256':sha(payload),'size':len(payload)},
        'source_commit':'8a66a55','comparisons':comparisons,'v13_exact_classifications':nonc['classifications'],
        'v13_exact_physical_transitions':revision['transitions'],'v13_resolved_anomalies':revision['resolved_anomalies'],
        'remaining_unknown_ranges':[row for row in proposal['ranges'] if row['kind']=='unknown'],
        'remaining_anomalies':proposal['anomalies'],'accepted_candidate_selected_address_intersections':intersections,
        'qualification':'No source/object/ABI/match or whole-ledger compatibility audit. Manifest intersections are exact metadata snapshots; active later changes are not approved. No adoption or C credit.'}
print(json.dumps(result,indent=2,sort_keys=True))
