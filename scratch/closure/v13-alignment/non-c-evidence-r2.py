"""Re-derive selected new non-C metadata, never emit private payloads."""
import hashlib
import json
from pathlib import Path
import struct
import sys
ROOT=Path(__file__).resolve().parents[3]
MAIN=ROOT.parents[2]
sys.path[:0]=[str(ROOT/'tools'),str(ROOT)]
from tools import analyze, catalogue_local_entries as local, catalogue_non_c as non_c, ctrfmt, fingerprint, image, inplace
sha=lambda data:hashlib.sha256(data).hexdigest()
raw=(MAIN/'scratch/review/catalogue-v12-r2/codex/proposal.json').read_bytes()
assert sha(raw)=='ee64b7d69bf5b96fe26a8ada241a59eca2924ac4ab9a376a5fc449e2d1e5a942'
baseline=json.loads(raw); functions={row['address']:row for row in baseline['functions']}
revision=json.loads((ROOT/'research/catalogue_v13_evidence.json').read_bytes())
source_path=MAIN/'scratch/research/library-candidate-triage/PROOF.json'
source=json.loads(source_path.read_bytes())
errno_path=MAIN/'scratch/research/thumb-profile/errno-anchor-evidence.json'
errno=json.loads(errno_path.read_bytes())
original=image.Image.load(ROOT/'orig'); analyzer=analyze.Analyzer(original.code,original.layout)
linker=inplace.InPlaceLinker(original,functions,inplace.Resolver(functions),(0,0x100000000))

def incoming(rows):
    result=[]
    for row in rows:
        address=row.get('owner',row.get('caller'))
        owner=functions[address]; site=row['site']; target=row.get('observed_target')
        flow=analyzer.decode(site,owner['isa'])
        assert flow and flow.call and flow.target[0]==target
        result.append({'site':site,'owner':address,'isa':owner['isa'],'record_sha256':local.metadata_sha(owner),
                       'image_sha256':sha(original.read(address,owner['size'])),'target':target,'target_isa':flow.target[1],'conditional':flow.conditional})
    return result

selected=[]
for name,record in source['locale'].items():
    row=next(row for row in record['attempts'] if row['equal'])
    selected.append((name,row,incoming(record['independent_placement'])))
for key,refs in [('unpack_three_complete_sections','independent_unpack_incoming'),
                 ('division_complete_section','independent_division_incoming'),
                 ('round_pack_complete_section','independent_round_pack_incoming'),
                 ('independent_complete_wrapper','independent_round_pack_incoming')]:
    selected.append((key,source['btod'][key],incoming(source['btod'][refs])))
for row in source['printf_collection']['sections']:
    if row['base']<0x100cd8:continue
    single={**row,'sections':[{'index':row['allocated_sections'][0]['index'],'base':row['base'],'size':row['size'],
                              'relocations':row['relocations']}], 'initial_placement':{str(row['allocated_sections'][0]['index']):row['base']},
            'placement_witnesses':[]}
    selected.append((row['member'],single,incoming(source['printf_collection']['independent_collection_entry'])))
selected.append(('errno',{**errno,'ar_header_offset':errno['member_offset'],'sections':[errno['section']],
                         'initial_placement':{'1':errno['section']['base']},'placement_witnesses':[]},incoming(errno['anchor']['incoming_references'])))
objects={}
requested={row['ar_header_offset']:row for _,row,_ in selected}
archive=ROOT/'.cache/armlib/5.04-b82/lib/armlib/c_5.l'
assert all(sha(archive.read_bytes())==row['archive_sha256'] for row in requested.values())
for member in fingerprint.archive_members(archive):
    row=requested.get(member.offset)
    if row is not None:
        assert member.name==row['member'] and sha(member.data)==row['member_sha256']
        objects[member.offset]=inplace.ObjectFile(Path(member.name),data=member.data)
    if objects.keys()==requested.keys():break
assert objects.keys()==requested.keys()
groups=[]
for identifier,row,refs in selected:
    obj=objects[row['ar_header_offset']]
    bases={section['index']:section['base'] for section in row['sections']}
    sections=[]
    for retained in row['sections']:
        sec=obj.sections[retained['index']]; base=retained['base']; data=bytearray(sec.data)
        assert len(sec.relocs)==len(retained['relocations'])
        relocs=[]
        for rel,old in zip(sec.relocs,retained['relocations']):
            assert (rel.offset,rel.type,rel.symbol.name)==(old['offset'],old['type'],old['symbol'])
            meta={'offset':rel.offset,'type':rel.type,'symbol':rel.symbol.name,'symbol_section':rel.symbol.shndx,
                  'symbol_value':rel.symbol.value,'symbol_offset':inplace._symbol_offset(rel.symbol),'symbol_size':rel.symbol.size,
                  'addend':linker._addend(rel,sec) if rel.type not in (0,40) else None}
            if old.get('rule')=='existing_inline_arm_call':
                callee=original.read(old['callee_address'],8)
                assert sha(callee)==old['callee_extent_sha256']
                decision=inplace.inline_arm_call(struct.unpack_from('<I',sec.data,rel.offset)[0],callee)
                assert decision and decision[0]=='inline_insn'
                struct.pack_into('<I',data,rel.offset,decision[1])
                meta.update(rule='existing_inline_arm_call',callee_address=old['callee_address'],callee_extent_sha256=sha(callee))
            elif rel.type in (0,40):
                meta['rule']='unchanged_marker'
            else:
                target=inplace.Target(old['target'],old['thumb'])
                same=isinstance(rel.symbol.shndx,int) and rel.symbol.shndx in bases
                if same:assert target.address==bases[rel.symbol.shndx]+inplace._symbol_offset(rel.symbol)
                linker._apply(rel,sec,data,base+rel.offset,target)
                meta.update(target=target.address,thumb=target.thumb,rule='same_object_definition' if same else 'observed_external_target_not_API_identity')
            relocs.append(meta)
        assert bytes(data)==original.read(base,sec.size)
        symbols=[{'name':s.name,'kind':s.kind,'bind':s.bind,'value':s.value,'offset':inplace._symbol_offset(s),'size':s.size}
                 for s in obj.symbols if s.shndx==sec.index and s.name not in ('$a','$t','$d')]
        sections.append({'index':sec.index,'base':base,'name':sec.name,'size':sec.size,'alignment':sec.align,
                         'executable':sec.executable,'writable':sec.writable,'nobits':sec.nobits,'symbols':symbols,
                         'mapping':[list(item) for item in sec.mapping],'object_sha256':sha(sec.data),
                         'relocated_sha256':sha(bytes(data)),'image_sha256':sha(original.read(base,sec.size)),
                         'whole_section_equal':True,'relocations':relocs})
    placements=[]
    for witness in row.get('placement_witnesses',[]):
        source_section=next(section for section in sections if section['index']==witness['from_section'])
        placements.append({'source_section':witness['from_section'],'offset':witness['site']-source_section['base'],
                           'target_section':witness['target_section']})
    groups.append({'id':identifier,'witness':{'archive':row['archive'],'archive_sha256':row['archive_sha256'],
                   'member':row['member'],'member_offset':row['ar_header_offset'],'member_sha256':row['member_sha256']},
                   'allocated_section_indices':sorted(obj.sections),'sections':sections,'placement':placements,'incoming':refs})

classifications=[]
for address,identity,coverage in [(0x100cd8,'ordered_printf_percent_collection_suffix',
                                  [(group['id'],section['index'],section['base'],section['base']+section['size'])
                                   for group in groups if group['id'].startswith('_printf_') for section in group['sections']]),
                                (0x107078,'_get_lc_numeric',[('func_00107078',3,0x107078,0x1070c0)]),
                                (0x134128,'_get_lc_ctype',[('func_00134128',3,0x134128,0x134170)]),
                                (0x1565f4,'_btod_d2e + _d2e_denorm_low + _d2e_norm_op1',
                                  [('unpack_three_complete_sections',1,0x1565f4,0x15662c),('unpack_three_complete_sections',5,0x15662c,0x156680),('unpack_three_complete_sections',3,0x156680,0x1566e8)]),
                                (0x1566e8,'__btod_div_common',[('division_complete_section',19,0x1566e8,0x1569b0)]),
                                (0x156a30,'_e2e',[('round_pack_complete_section',14,0x156a30,0x156b0c)]),
                                (0x181e94,'__read_errno',[('errno',1,0x181e94,0x181e9e)])]:
    fn=functions[address]
    record={'address':address,'size':fn['size'],'isa':fn['isa'],'before_sha256':local.metadata_sha(fn),
            'extent_sha256':sha(original.read(address,fn['size'])),'kind':'library','identity':identity,'coverage':coverage,
            'qualification':'Complete selected input sections and independent original placement; no new API aliases, historical archive uniqueness, pool/data ownership, extent change or C credit. Observed external targets are not API identities.'}
    if address==0x181e94:
        anchor=errno['anchor']; retained=functions[anchor['address']]
        assert local.metadata_sha(retained)==anchor['record_sha256']
        record['anchor']={'address':anchor['address'],'symbol':anchor['symbol'],'offset':anchor['symbol_offset'],
                          'record_sha256':local.metadata_sha(retained),'group':'errno','section':1}
    classifications.append(record)
tramp=source['trampoline']; fn=functions[tramp['address']]
classifications.append({'address':fn['address'],'size':fn['size'],'isa':fn['isa'],'before_sha256':local.metadata_sha(fn),
                        'extent_sha256':sha(original.read(fn['address'],fn['size'])),'kind':'assembly','identity':'fixed_LR_continuation_trampoline',
                        'control_transfer':{'literal_site':tramp['literal_site'],'continuation':tramp['continuation'],
                                            'incoming':incoming(tramp['incoming_calls'])},
                        'qualification':'Exact architecture-specific fixed LR continuation and terminal non-call indirect transfer; no vendor/API identity or ordinary C calling-convention claim.'})
classifications.sort(key=lambda row:row['address'])
proof={'schema_version':1,'image_sha256':original.sha256,'baseline':revision['baseline'],'groups':groups,'classifications':classifications,
       'source_evidence':[{'path':str(path.relative_to(MAIN)),'sha256':sha(path.read_bytes())} for path in (source_path,errno_path)]}
revision['_non_c_proof']=proof
revision['non_c_proof']={'path':'research/catalogue_v13_non_c_proof.json','sha256':local.metadata_sha(proof)}
non_c._check(baseline,analyzer,revision)
recheck=non_c.prove_objects(ROOT,baseline,analyzer,revision)
assert recheck['complete_section_bytes']>1500
print(json.dumps(proof,indent=2,sort_keys=True))
