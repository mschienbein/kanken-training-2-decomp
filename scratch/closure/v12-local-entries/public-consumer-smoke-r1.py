
from copy import deepcopy
import hashlib, json, pathlib, subprocess, sys, tempfile
from tools import analyze, catalogue_closure, catalogue_local_entries as local, ctrfmt
from tests.test_catalogue_local_entries import local_fixture, build
from tests.test_catalogue_protection import synthetic_project, run_catalogue, write_json
root = pathlib.Path.cwd()
python = sys.executable
baseline_path = root.parent / 'catalogue-v11-r4/scratch/analyze/catalogue-v11-r4.json'
private_command = [python, '-B', 'tools/catalogue_local_entries.py', '--baseline', str(baseline_path), '--prove-objects']
private = subprocess.run(private_command, cwd=root, capture_output=True, text=True, check=False)
assert private.returncode == 0, private.stderr
private_result = json.loads(private.stdout)
assert private_result['object_proof']['complete_section_bytes'] == 96
# Public synthetic producer and consumer; no unittest runner or mocks.
data = local_fixture()
synthetic = build(data)
analyzer = analyze.Analyzer(data['image'], data['layout'])
restored = local.check_catalogue(synthetic, analyzer, data['closure'])
assert local.baseline_hashes(restored) == local.baseline_hashes(data['baseline'])
assert analyze.validate_catalogue(synthetic, data['image'], data['layout']) == []
assert synthetic['stats']['creditable_functions'] == data['baseline']['stats']['creditable_functions']
assert synthetic['stats']['creditable_bytes'] == data['baseline']['stats']['creditable_bytes']
assert synthetic['closure_evidence']['read_vetoes'] == data['baseline']['closure_evidence']['read_vetoes']
assert not synthetic['closure_evidence']['local_runtime_entries']['runtime_pool_value_admitted']
refusals = []
for change in ('member', 'anchor', 'mapping', 'relocations', 'size', 'tag', 'section_hash'):
    altered = deepcopy(data)
    revision = altered['closure']['local_entry_revision']
    record = revision['_object_proof']['records'][0]
    if change == 'member': record['member'] = 'other.o'
    elif change == 'anchor': record['anchor']['symbol'] = 'unproved_alias'
    elif change == 'mapping': record['section']['mapping'] = [[0, '$t']]
    elif change == 'relocations': record['section']['relocations'].pop()
    elif change == 'size': record['size'] = 12
    elif change == 'tag': record['section']['symbols'][1]['value'] = 0
    else: record['section']['image_sha256'] = hashlib.sha256(b'wrong section').hexdigest()
    revision['object_proof']['sha256'] = local.metadata_sha(revision['_object_proof'])
    try: build(altered)
    except ValueError: refusals.append(change)
    else: raise AssertionError('synthetic certificate was admitted: ' + change)
for change in ('old_pointer_record', 'missing_veto', 'hole', 'credit'):
    altered = deepcopy(synthetic)
    if change == 'old_pointer_record':
        altered['functions'] = [deepcopy(data['previous']) if row['address'] == 0x100020 else row for row in altered['functions']]
    elif change == 'missing_veto': altered['closure_evidence']['read_vetoes'] = []
    elif change == 'hole': next(row for row in altered['ranges'] if row['start'] == 0x10004a)['kind'] = 'rodata'
    else: altered['stats']['creditable_bytes'] += 10
    try: local.check_catalogue(altered, analyzer, data['closure'])
    except ValueError: refusals.append(change)
    else: raise AssertionError('synthetic candidate was admitted: ' + change)
with tempfile.TemporaryDirectory() as directory:
    temporary = pathlib.Path(directory)
    revision = data['closure']['local_entry_revision']
    archive = temporary / revision['_object_proof']['records'][0]['archive']
    archive.parent.mkdir(parents=True)
    archive.write_bytes(data['archive'])
    synthetic_objects = local.prove_objects(temporary, data['baseline'], analyzer, revision)
    assert synthetic_objects['complete_section_bytes'] == 48
    archive.write_bytes(data['archive'] + b'changed')
    try: local.prove_objects(temporary, data['baseline'], analyzer, revision)
    except ValueError: refusals.append('private_member_archive_hash')
    else: raise AssertionError('wrong synthetic archive was admitted')
with tempfile.TemporaryDirectory() as directory:
    temporary = pathlib.Path(directory)
    synthetic_project(temporary)
    cli = run_catalogue(temporary)
    assert cli.returncode == 0, cli.stderr
    candidate = json.loads((temporary / 'proposal.json').read_bytes())
    assert candidate['generator']['revision'] == 'catalogue-v12'
    child_path = temporary / 'research/catalogue_v12_evidence.json'
    child = json.loads(child_path.read_bytes())
    child['parent_evidence']['sha256'] = hashlib.sha256(b'wrong parent').hexdigest()
    write_json(child_path, child)
    rejected_cli = run_catalogue(temporary, 'refused.json')
    assert rejected_cli.returncode == 1 and not (temporary / 'refused.json').exists(), rejected_cli.stderr
# The complete retained r4 artifact is consumed in memory only. No discovery,
# current-image generation or successor catalogue file is performed/written.
evidence, _, _ = catalogue_closure.load_inputs(root, root / 'research/catalogue_v9_r2_evidence.json')
image = (root / 'orig/code.bin').read_bytes()
layout = ctrfmt.parse_exheader((root / 'orig/exheader.bin').read_bytes())
analyzer = analyze.Analyzer(image, layout)
retained = json.loads(baseline_path.read_bytes())
previous_functions = {row['address']: local.metadata_sha(row) for row in retained['functions']}
previous_ordinary = {row['address']: row['size'] for row in retained['functions'] if row['kind'] == 'function'}
previous_unknown = retained['stats']['bytes_by_range_kind']['unknown']
catalogue_closure.check_entry_revision(retained, analyzer, evidence, {}, evidence['classification_records'])
local.close_catalogue(retained, analyzer, evidence, {})
restored = local.check_catalogue(retained, analyzer, evidence)
assert local.baseline_hashes(restored) == evidence['local_entry_revision']['baseline']['field_sha256']
actual_functions = {row['address']: row for row in retained['functions']}
assert {address: local.metadata_sha(actual_functions[address]) for address in previous_functions} == previous_functions
assert {row['address']: row['size'] for row in retained['functions'] if row['kind'] == 'function'} == previous_ordinary
assert set(actual_functions) - previous_functions.keys() == {0x1005c0, 0x1009d4}
assert all((actual_functions[address]['kind'], actual_functions[address]['size']) == ('library', 10) for address in (0x1005c0, 0x1009d4))
assert previous_unknown - retained['stats']['bytes_by_range_kind']['unknown'] == 20
for lo, hi in evidence['local_entry_revision']['preserved_unknown_ranges']:
    assert {'start': lo, 'end': hi, 'kind': 'unknown', 'owner': None} in retained['ranges']
print(json.dumps({'private_complete_section_cli': {'actual_exit_code': private.returncode, 'result': private_result},
                  'synthetic_public_builder_and_consumer': 'pass', 'synthetic_complete_real_elf_bytes': 48,
                  'synthetic_refusals': refusals, 'real_synthetic_cli': {'actual_exit_code': cli.returncode,
                  'refused_actual_exit_code': rejected_cli.returncode, 'refused_output_exists': False},
                  'retained_r4_metadata_consumer': {'new_library_entries': ['001005c0/10', '001009d4/10'],
                  'unchanged_complete_old_records': len(previous_functions), 'unchanged_ordinary_identities': len(previous_ordinary),
                  'ordinary_extent_bytes': sum(previous_ordinary.values()), 'functions': retained['stats']['functions'],
                  'unknown_bytes': retained['stats']['bytes_by_range_kind']['unknown'], 'unknown_intervals': retained['stats']['unknown_ranges'],
                  'anomalies': retained['stats']['anomalies'], 'new_C_credit': 0,
                  'fresh_current_image_generation': False, 'successor_catalogue_written': False}}, indent=2, sort_keys=True))
