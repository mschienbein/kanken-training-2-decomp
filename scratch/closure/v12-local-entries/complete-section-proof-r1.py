
import hashlib, json, pathlib, struct, sys
root = pathlib.Path.cwd()
sys.path.insert(0, str(root / 'tools'))
import analyze, catalogue_closure, fingerprint, image, inplace
sha = lambda data: hashlib.sha256(data).hexdigest()
meta = lambda value: sha(analyze.serialize_catalogue(value))
p = root.parent / 'catalogue-v11-r4/scratch/analyze/catalogue-v11-r4.json'
blob = p.read_bytes()
assert sha(blob) == '55168a79db0d76debddd3b55cf62beed6ab29dc73396d2d3dd29e882ac6e3451', 'fresh r4 proposal digest'
baseline = json.loads(blob)
original = image.Image.load(root / 'orig')
assert original.sha256 == baseline['image_sha256'], 'original image digest'
closure, adopted, inputs = catalogue_closure.load_inputs(root, root / 'research/catalogue_v9_r2_evidence.json')
assert baseline['generator']['revision'] == closure['revision'] == 'catalogue-v11-r4'
for label, digest in inputs.items():
    assert baseline['generator']['input_sha256'][label] == digest, 'r4 frozen input binding'
for label, digest in baseline['generator']['source_sha256'].items():
    assert sha((root / label).read_bytes()) == digest, 'r4 source binding'
assert baseline['revision_review']['leaf_mode_repair'] == closure['leaf_mode_repair']
analyzer = analyze.Analyzer(original.code, original.layout)
before = meta(baseline['closure_evidence']['provisional_entry_reassessment'])
catalogue_closure.check_entry_revision(baseline, analyzer, closure, {}, closure['classification_records'])
assert meta(baseline['closure_evidence']['provisional_entry_reassessment']) == before, 'r4 negative-stage exact consumer'
functions = {row['address']: row for row in baseline['functions']}
library = closure['library_identification']
archive = root / '.cache/armlib/5.04-b82/lib/armlib/c_5.l'
archive_sha = sha(archive.read_bytes())
assert archive_sha == '3583239cecab63fb14cd40478ffb4d6fbedd67abd1df5acaa332d47d274d1fbe'
scout_metadata = json.loads((root.parent / 'catalogue-v11/scratch/runtime-data-v11/alignment-object-metadata.json').read_bytes())
if isinstance(scout_metadata, dict):
    rows = next(value for value in scout_metadata.values() if isinstance(value, list) and value and isinstance(value[0], dict) and 'member' in value[0])
else:
    rows = scout_metadata
linker = inplace.InPlaceLinker(original, functions, inplace.Resolver(functions), (0, 0x100000000))
requested = {656918: (0x1005c0, '_printf_input_char', '_printf_char_common'), 670346: (0x1009d4, '_printf_input_wchar', '_printf_wchar_common')}
records = []
for member in fingerprint.archive_members(archive):
    if member.offset not in requested:
        continue
    base, local_name, anchor_name = requested[member.offset]
    old = next(row for row in rows if row['member_offset'] == member.offset)
    assert member.name == old['member'] and sha(member.data) == old['member_sha256']
    obj = inplace.ObjectFile(pathlib.Path(member.name), data=member.data)
    assert len(obj.sections) == 1, 'complete allocated section inventory'
    sec = obj.sections[1]
    section_meta = {'index': 1, 'name': sec.name, 'alignment': sec.align, 'size': sec.size,
                    'executable': sec.executable, 'writable': sec.writable,
                    'mapping': [list(pair) for pair in sec.mapping],
                    'symbols': [{'name': s.name, 'kind': s.kind, 'bind': s.bind, 'value': s.value,
                                 'offset': inplace._symbol_offset(s), 'size': s.size}
                                for s in obj.symbols if s.shndx == 1 and s.name not in ('$t', '$d', '$a')],
                    'relocations': [{'offset': r.offset, 'type': r.type, 'symbol': r.symbol.name,
                                     'symbol_section': r.symbol.shndx, 'symbol_value': r.symbol.value,
                                     'symbol_size': r.symbol.size} for r in sec.relocs]}
    assert section_meta == old['sections'][0], 'all original section metadata'
    assert sec.size == 48 and sec.align == 4 and not sec.nobits and sec.executable and not sec.writable
    assert sec.mapping == [(0, '$t'), (42, '$d')]
    local = next(s for s in obj.symbols if s.name == local_name)
    anchor_sym = next(s for s in obj.symbols if s.name == anchor_name)
    assert (local.kind, local.bind, local.shndx, local.value, local.size) == ('STT_FUNC', 'STB_LOCAL', 1, 1, 10)
    assert (anchor_sym.kind, anchor_sym.bind, anchor_sym.shndx, anchor_sym.value, anchor_sym.size) == ('STT_FUNC', 'STB_GLOBAL', 1, 11, 32)
    anchor = functions[base + 10]
    authentication = next(row for row in library['authenticated_owners'] if row['address'] == anchor['address'])
    assert authentication['authenticated'] and authentication['whole_body_identical']
    assert authentication['selected_alias_groups'] == [[anchor_name]] and authentication['incoming_library_symbol_witnesses']
    assert anchor['classification_evidence']['identity'] == anchor_name
    witness = anchor['classification_evidence']['library_witness']
    assert (witness['archive_member'], witness['member_offset'], witness['member_sha256'], witness['symbol']) == (member.name, member.offset, sha(member.data), anchor_name)
    assert sha(original.read(anchor['address'], 38)) == witness['image_body_sha256']
    incoming = []
    for site in anchor['entries'][0]['sources']:
        owners = [fn for fn in functions.values() if any(lo <= site < hi for lo, hi in fn['code_ranges'])]
        assert len(owners) == 1
        flow = analyzer.decode(site, owners[0]['isa'])
        assert flow and flow.call and flow.target == (anchor['address'], 'thumb')
        incoming.append({'site': site, 'caller': owners[0]['address'], 'caller_record_sha256': meta(owners[0]),
                         'caller_image_sha256': sha(original.read(owners[0]['address'], owners[0]['size'])), 'isa': owners[0]['isa']})
    relocated = bytearray(sec.data)
    relocation_rows = []
    assert [(r.offset, r.type, r.symbol.name) for r in sec.relocs] == [(34, 10, '__printf'), (44, 3, local_name)]
    for rel in sec.relocs:
        place = base + rel.offset
        addend = linker._addend(rel, sec)
        if rel.type == 3:
            target = inplace.Target(base + inplace._symbol_offset(rel.symbol), True)
            agreement = 'same_section_definition'
            assert rel.symbol is local
        else:
            flow = analyzer.decode(place, 'thumb')
            assert flow and flow.call and not flow.conditional and flow.target == (0x10064c, 'thumb')
            inferred = (linker._original_value(rel, place, sec) - addend) & 0xffffffff
            target = inplace.Target(inferred & ~1, True)
            assert target.address == flow.target[0]
            agreement = 'observed_external_target_not_API_identity'
        linker._apply(rel, sec, relocated, place, target)
        assert bytes(relocated[rel.offset:rel.offset + 4]) == original.read(place, 4), 'individual reencoding'
        relocation_rows.append({**section_meta['relocations'][len(relocation_rows)], 'addend': addend,
                                'target': target.address, 'thumb': target.thumb, 'agreement': agreement, 'reencoded_equal': True})
    expected = original.read(base, 48)
    assert bytes(relocated) == expected, 'complete independently placed 48-byte section equality'
    assert sec.data[:10] == expected[:10] and bytes(relocated[10:]) == expected[10:]
    composition = []
    for lo, hi, role in [(0, 10, 'local_declared_function'), (10, 42, 'anchor_declared_function'),
                         (42, 44, 'mapped_data_halfword_unclassified'), (44, 48, 'same_section_relative_relocation')]:
        composition.append({'offset': lo, 'end_offset': hi, 'role': role,
                            'object_sha256': sha(sec.data[lo:hi]), 'image_sha256': sha(expected[lo:hi]),
                            'relocated_sha256': sha(bytes(relocated[lo:hi])), 'reencoded_equal': True})
    records.append({'address': base, 'size': 10, 'isa': 'thumb', 'symbol': local_name,
                    'archive': '.cache/armlib/5.04-b82/lib/armlib/c_5.l', 'archive_sha256': archive_sha,
                    'member': member.name, 'member_offset': member.offset, 'member_sha256': sha(member.data),
                    'section': {**section_meta, 'allocated': True, 'nobits': False,
                                'object_sha256': sha(sec.data), 'image_sha256': sha(expected),
                                'relocated_sha256': sha(bytes(relocated)), 'whole_section_reencoded_equal': True,
                                'relocations': relocation_rows, 'composition': composition},
                    'fragment_sha256': sha(expected[:10]),
                    'anchor': {'address': base + 10, 'size': 38, 'isa': 'thumb', 'symbol': anchor_name,
                               'symbol_offset': 10, 'declared_size': 32, 'record_sha256': meta(anchor),
                               'image_sha256': sha(original.read(base + 10, 38)),
                               'classification_sha256': meta(anchor['classification_evidence']),
                               'authentication_sha256': meta(authentication), 'incoming_references': incoming},
                    'unclassified_halfword': [base + 42, base + 44]})
assert len(records) == 2
fields = ('functions', 'ranges', 'stats', 'anomalies', 'pc_relative_pointer_witnesses', 'closure_evidence')
print(json.dumps({'scope': 'two exact members,96 section bytes; no discovery or current-image generation',
                  'image_sha256': original.sha256, 'exheader_sha256': sha((root / 'orig/exheader.bin').read_bytes()),
                  'baseline': {'proposal_sha256': sha(blob), 'revision': 'catalogue-v11-r4', 'adopted': False,
                               'field_sha256': {key: meta(baseline[key]) for key in fields},
                               'independent_r3_consumer': True, 'r4_source_and_input_binding': True},
                  'records': records}, indent=2, sort_keys=True))
