"""Bounded exact-member reconstruction; stdout contains metadata and hashes only."""
from __future__ import annotations
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(ROOT / 'tools'))
from tools import analyze, catalogue_alignment as alignment, catalogue_local_entries as local, ctrfmt, fingerprint, image, inplace

MAIN = ROOT.parents[2]
BASELINE = MAIN / 'scratch/review/catalogue-v12-r2/codex/proposal.json'
PREFIX = MAIN / 'scratch/research/catalogue-residual-prefix/EVIDENCE.json'
TAIL = MAIN / 'scratch/research/catalogue-residual-tail/probe-r4.json'
sha = lambda data: hashlib.sha256(data).hexdigest()
raw = BASELINE.read_bytes()
assert sha(raw) == 'ee64b7d69bf5b96fe26a8ada241a59eca2924ac4ab9a376a5fc449e2d1e5a942'
baseline = json.loads(raw)
functions = {row['address']: row for row in baseline['functions']}
prefix, tail = json.loads(PREFIX.read_bytes()), json.loads(TAIL.read_bytes())
original = image.Image.load(ROOT / 'orig')
analyzer = analyze.Analyzer(original.code, original.layout)
linker = inplace.InPlaceLinker(original, functions, inplace.Resolver(functions), (0, 0x100000000))
base_descriptor = {'revision': 'catalogue-v12-r2', 'adopted': False, 'proposal_sha256': sha(raw),
                   'field_sha256': alignment.baseline_hashes(baseline),
                   'range_kinds': sorted(baseline['stats']['bytes_by_range_kind'])}
external = [0x100076,0x10051e,0x10090a,0x100942,0x100a6a,0x100b26,0x100c92,
            0x16b6a2,0x181eaa,0x1911ee,0x191c92,0x19d5c2,0x19f6d6,0x19fd6a,0x19ffa2]
internal = [(0x100396,0x100360,'REL32',0x10039c),(0x1005ea,0x1005ca,'REL32',0x1005f0),
            (0x1007ce,0x10064c,'REL32',0x1007d4),(0x1008c6,0x1008ac,'REL32',0x1008cc),
            (0x1009fe,0x1009de,'REL32',0x100a04),(0x100ada,0x100ad0,'retained_literal',0x100ae4),
            (0x100c7a,0x100bb0,'REL32',0x100c80),(0x19f622,0x19f5c4,'retained_literal',0x19f634),
            (0x19f696,0x19f63c,'retained_literal',0x19f6a8)]
selected = []
for obj in prefix['objects']:
    for section in obj['allocated_sections']:
        base = section['section_base']
        if base is not None and section.get('whole_section_reencoded_equal') and (
                base + section['size'] in external or any(base <= start < base+section['size'] for start,_,_,_ in internal)):
            selected.append({'base': base,'index':section['index'],'size':section['size'],'alignment':section['alignment'],
                             'witness':{'archive':obj['archive'],'archive_sha256':obj['archive_sha256'],'member':obj['member'],
                                        'member_offset':obj['member_header_offset'],'member_sha256':obj['member_sha256']},
                             'relocations':section['relocations'],'source':{'path':str(PREFIX.relative_to(MAIN)),
                              'sha256':sha(PREFIX.read_bytes()),'member':obj['member'],'section':section['index']}})
for obj in tail['library_sections']:
    for section in obj['sections']:
        base = section['base']
        if base+section['size'] in external or any(base <= start < base+section['size'] for start,_,_,_ in internal):
            selected.append({'base':base,'index':section['index'],'size':section['size'],'alignment':section['alignment'],
                             'witness':{key:obj[key] for key in ('archive','archive_sha256','member','member_offset','member_sha256')},
                             'relocations':section['relocations'],'source':{'path':str(TAIL.relative_to(MAIN)),
                              'sha256':sha(TAIL.read_bytes()),'member':obj['member'],'section':section['index']}})
thumb_path = MAIN / 'scratch/research/thumb-profile/complete-section-evidence.json'
thumb_record = next(row for row in json.loads(thumb_path.read_bytes())['records'] if row['address'] == 0x19d5b0)
thumb_section = thumb_record['section']
selected.append({'base':thumb_section['base'],'index':thumb_section['index'],'size':thumb_section['size'],'alignment':thumb_section['alignment'],
                 'witness':{key:thumb_record[key] for key in ('archive','archive_sha256','member','member_offset','member_sha256')},
                 'relocations':thumb_section['relocations'],
                 'source':{'path':str(thumb_path.relative_to(MAIN)),'sha256':sha(thumb_path.read_bytes()),'member':'wcslen.o','section':1}})
sections=[]
for archive in sorted({row['witness']['archive'] for row in selected}):
    requests=[row for row in selected if row['witness']['archive']==archive]
    content=(ROOT/archive).read_bytes()
    assert all(sha(content)==row['witness']['archive_sha256'] for row in requests)
    wanted={row['witness']['member_offset']:row for row in requests}
    objects={}
    for member in fingerprint.archive_members(ROOT/archive):
        if member.offset not in wanted:
            continue
        witness=wanted[member.offset]['witness']
        assert member.name==witness['member'] and sha(member.data)==witness['member_sha256']
        objects[member.offset]=inplace.ObjectFile(Path(member.name),data=member.data)
        if len(objects)==len(wanted):
            break
    assert objects.keys()==wanted.keys()
    for row in requests:
        obj=objects[row['witness']['member_offset']]
        section=obj.sections[row['index']]
        assert (section.size,section.align)==(row['size'],row['alignment'])
        assert len(section.relocs)==len(row['relocations'])
        symbols=[{'name':s.name,'kind':s.kind,'bind':s.bind,'value':s.value,'offset':inplace._symbol_offset(s),'size':s.size}
                 for s in obj.symbols if s.shndx==row['index'] and s.name not in ('$a','$t','$d')]
        relocated=bytearray(section.data); relocations=[]
        for relocation, retained in zip(section.relocs,row['relocations']):
            assert (relocation.offset,relocation.type,relocation.symbol.name)==(retained['offset'],retained['type'],retained['symbol'])
            addend=linker._addend(relocation,section)
            if 'addend' in retained: assert addend==retained['addend']
            target=inplace.Target(retained['target'],retained['thumb'])
            if relocation.symbol.shndx==row['index']:
                assert target.address==row['base']+inplace._symbol_offset(relocation.symbol)
                assert target.thumb==bool(relocation.symbol.value&1)
            linker._apply(relocation,section,relocated,row['base']+relocation.offset,target)
            width = 2 if relocation.type in (102,103) else 4
            assert bytes(relocated[relocation.offset:relocation.offset+width])==original.read(row['base']+relocation.offset,width)
            relocations.append({'offset':relocation.offset,'type':relocation.type,'width':width,'symbol':relocation.symbol.name,
                                'symbol_section':relocation.symbol.shndx,'symbol_value':relocation.symbol.value,
                                'symbol_size':relocation.symbol.size,'addend':addend,'target':target.address,'thumb':target.thumb,
                                'agreement':retained.get('agreement',retained.get('basis')),'reencoded_equal':True})
        assert bytes(relocated)==original.read(row['base'],section.size)
        boundaries={0,section.size}
        boundaries.update(o for o,_ in section.mapping)
        for symbol in symbols:
            if symbol['size'] and symbol['kind'] in ('STT_FUNC','STT_OBJECT'):
                boundaries.update((symbol['offset'],symbol['offset']+symbol['size']))
        for relocation in relocations: boundaries.update((relocation['offset'],relocation['offset']+relocation['width']))
        for start,_,_,field_end in internal:
            if row['base']<=start<row['base']+section.size:
                boundaries.update((start-row['base'],start+2-row['base'],min(field_end-row['base'],section.size)))
        composition=[]
        for lo,hi in zip(sorted(boundaries),sorted(boundaries)[1:]):
            composition.append({'offset':lo,'end_offset':hi,'object_sha256':sha(section.data[lo:hi]),
                                'image_sha256':sha(original.read(row['base']+lo,hi-lo)),
                                'relocated_sha256':sha(bytes(relocated[lo:hi])),'reencoded_equal':True})
        bindings=[{'address':f['address'],'record_sha256':local.metadata_sha(f)} for f in baseline['functions']
                  if row['base']<=f['address']<row['base']+section.size]
        references=[]
        for index,(offset,kind) in enumerate(section.mapping):
            if kind not in ('$a','$t'): continue
            end=section.mapping[index+1][0] if index+1<len(section.mapping) else section.size
            decoder=analyzer.arm if kind=='$a' else analyzer.thumb
            instructions=list(decoder.disasm(bytes(relocated[offset:end]),row['base']+offset))
            assert sum(ins.size for ins in instructions)==end-offset
            for ins in instructions:
                flow=analyzer.decode(ins.address,'arm' if kind=='$a' else 'thumb')
                assert flow is not None
                for lo,hi in flow.pools:
                    references.append({'site':ins.address,'start':lo,'end':hi,'kind':'bounded_direct_pc_read'})
        sections.append({'id':f"{row['base']:08x}:{row['index']}:{row['witness']['member']}",
                         'base':row['base'],'index':row['index'],'name':section.name,'size':section.size,'alignment':section.align,
                         'allocated':True,'executable':section.executable,'writable':section.writable,'nobits':section.nobits,
                         'complete':True,'whole_section_reencoded_equal':True,'object_sha256':sha(section.data),
                         'image_sha256':sha(bytes(relocated)),'relocated_sha256':sha(bytes(relocated)),
                         'mapping':[list(item) for item in section.mapping],'symbols':symbols,'relocations':relocations,
                         'composition':composition,'bindings':bindings,'bounded_references':references,
                         'witness':row['witness'],'source':row['source']})
sections.sort(key=lambda row:row['base'])
census=[]; fill=None
for start in external:
    end=start+2; value=original.read(start,2)
    if fill is None: fill=value
    assert value==fill
    successor=functions[end]
    predecessor=next((row for row in sections if row['base']+row['size']==start),None)
    census.append({'start':start,'end':end,'image_sha256':sha(value),'successor':end,
                   'successor_alignment':4,'successor_record_sha256':local.metadata_sha(successor),
                   'alignment_authority':'architectural_arm_state' if successor['isa']=='arm' else 'complete_object_section',
                   'successor_section':None,'predecessor_section':predecessor['id'] if predecessor else None,
                   'missing_predecessor_proof':None if predecessor else
                   'weak_import_field_refused' if start in (0x100942,0x100b26) else 'no_complete_matching_predecessor_object'})
assert len(census)==15 and sum(row['predecessor_section'] is not None for row in census)==13
internal_rows=[]
for start,owner,kind,field_end in internal:
    section=next(row for row in sections if row['base']<=start<row['base']+row['size'])
    assert not any(original.read(start,2))
    internal_rows.append({'start':start,'end':start+2,'owner':owner,'section':section['id'],
                          'field_alignment':4,'field_kind':kind,'field_end':field_end})
proof={'schema_version':1,'image_sha256':sha(original.code),'baseline':base_descriptor,
       'sources':[{'path':str(path.relative_to(MAIN)),'sha256':sha(path.read_bytes())} for path in (PREFIX,TAIL)],
       'sections':sections,'external_census':census,'external_census_intervals':[[row['start'],row['end']] for row in census],
       'internal_padding':internal_rows,'uniform_fill_observations':15,'uniform_fill_sha256':sha(fill),
       'qualification':'No payload, runtime pool-value admission, inferred API identity or historical archive uniqueness. Two incomplete weak-import predecessors remain unknown; wcslen section geometry does not admit its API identity.'}
print(json.dumps(proof,indent=2,sort_keys=True))
