# ctypes1 — rebased type composition and qualifications

## c010-r8 rebase

This unaccepted candidate is based on c010-r8 source
`f78df90632d86c1f48eb90b4ca2aacc5a23b50bc`. The complete `types-compose`
history through `38b101bb` is retained by merge ancestry. Unsupported active
cutovers are explicitly held, not silently reintroduced through aliases.

The incoming manifest contains **83 units / 85 claims**. Of those, **79 units**
are held at the c010 forms, two native reader providers are already owned by
c010, and two units remain in the manifest: the **4-byte DisplayFramebuffer
constructor move** and **NEW native Font::GetCharStrmReader (64 bytes)**.
All **2,682 c010-r8 identities** survive; the dry proposal has **2,683 functions**.
The c010 callback/thread/atexit corrections and all **94 NEW-only deferrals**
take precedence. No accepted or c010 identity is deferred by this composition.

### Closed retained work

- Independent `InputRectangle` providers/callers use actual rectangle members,
  without a MenuScene, MenuControlSet or alternate display-owner conversion.
- Serialized font data uses the shared records while preserving c010's const
  width/map pointers, mutable cache halfwords and canonical field names.
- The display-service framebuffer constructor has its actual value-object
  receiver and original two-element/16-byte lifetime evidence. No explicit
  incompatible callback cast or specialized array-runtime declaration is added.
- Native `DeleteTexture(const TextureInfo&)` uses the constructed native record,
  not a first-word surrogate. Its release callback protocol remains unchanged.
- One `GpuState` retains c010's requested/excluded flags, feature byte and both
  33-word constant arrays. Independent shader fields occupy only previously
  unused padding. The graphics-context buffer pair/completion event are retained.
- The NEW reader keeps the native const receiver, hidden aggregate result and
  unused `wchar_t` input. It selects only the direct native UTF16 member or null;
  it does not complete the game `FontVTable` or select the inherited stream-step
  helper. Historical identity/TU spelling remains `[INFERENCE]`.

### Explicit held transactions

- **Voice / RecordState / CWAR / envelope owner changes:** competing receiver,
  array-callback and producer contracts cannot be joined to c010's actual
  `AudioVoiceGroup`, `AudioSlot`, `AudioEnvelope` and `ArchiveReader` clients by
  casts or same-layout aliases. Their c010 forms remain.
- **Full game display and TextBox pool cutover:** c010 has additional credited
  `BoundDisplayObject`, pending-update, material and target-version text-state
  clients. The prior ResourceEntry/TextBox cleanup improvement is not claimed
  complete while those clients retain a different dirty/state protocol. The
  entire dependent transaction, including the proposed `0015e5d4` re-entry,
  stays at c010 forms rather than reopening prereview F02.
- **Filesystem global and DSP projections:** c010's single `ServiceClientState`
  for `data_00242c18`, real `AudioDspParameterBlock.volume` and coordinator audio
  storage win over the competing globals/subobject views.
- **Native reader helper cleanup:** advancing `m_CharStrm` without pointer-
  reference punning perturbed c010's live SJIS provider. Two natural typed-cursor
  forms also mismatched (56 and 48 bytes versus 56). The header and provider were
  restored exactly to c010. This does not newly approve the inherited reference
  cast or assert that it has been repaired. The NEW reader introduces no use of
  that helper; its selected UTF16 provider advances actual pointer values.
- **Material BlendMode and MenuScene:** the earlier material-family rollback
  remains. No unsigned-backed BlendMode, packed-word reinterpretation or
  canonical-display bridge is adopted. The native MenuScene transaction remains
  quarantined until an independently reviewed linker U1 model is adopted.

`00131540`, `001f4920`, `001efcec`, `001f4994`, `00117854` and `001e84ec`
remain unselected. `FontVTable` / `data_00223d98` remain opaque. Converter cells
retain `float(float)`. No bound native slot receives a newly invented free
provider, and no complete base wrapper is downcast to a derived value.

### Evidence and gate boundary

The rebase changes **23 source units and eight headers**. Its final affected
closure has **1,438 primary units / 1,466 functions**, all MATCH, with ten
helper-only definitions proved in their natural contexts. Every invocation is
sequential, niced, single-process `tools/match.py --show 0`, using the frozen
profile and explicit v6/L1. Failed or superseded attempts remain immutable.

Entry point: `scratch/integrator/ctypes1/r2/HANDOFF-r1.json`; detailed holds and
conflicts, dry proposed ledger, source-bound proofs and complete rewrite
projection are in that directory. MATCH is not interface or source-policy
acceptance. No whole-ledger interfaces/argcheck, full build, suite, prepare,
review registration, finalization or push was run by this worker.

The separate accepted-runtime callback attention in c010-r8 is not waived by
this byte proof. Main owns the final callback adjudications and all heavy gates
once the c010 predecessor is finalized and the integrated inputs are committed.
