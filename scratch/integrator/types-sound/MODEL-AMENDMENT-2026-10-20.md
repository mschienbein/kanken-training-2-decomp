# Sound MODEL amendment — 2026-10-20

Applies to `scratch/research/type-models/sound/MODEL.md`, especially the
`RecordState` layout in S03. This is a correction to source types, not a
compiler-profile, linker-model, literal-model, gate or credit change.

## RecordState +0x08 is a loop-context pointer

Replace the inferred integer `sample_offset` at +0x08 with:

```cpp
const AdpcmLoopState *loop_context;
```

Original `func_00153af8` passes a real per-channel ADPCM loop-state destination
to `func_0017da60` at `0x00153bcc`, clears the real record through
`func_0017da3c` at `0x00153bd4`, then stores **the same live pointer** at record
+0x08 at `0x00153bd8`. The record base used by that store is the exact receiver
of the clear call. Thus this word is not an integer sample offset, a token or
a pointer represented as an integer. The record remains 0x18 bytes.

This was surfaced in `scratch/authors/ns100/RESULT.json` and independently
re-derived from the original image. Instruction-independent address, offset,
hash and value-origin evidence is retained in
`loop-context-provenance-r1.json` next to this amendment. The field name and
read-only source qualifier are [INFERENCE]; pointer provenance and storage
position are directly established. No MATCH or admission of `00153af8` is
claimed: its author result remains a mismatch.

`AdpcmLoopState` is defined once in the shared waveform header and is used
by waveform-channel initial/loop histories, the real stream-channel loop-state
array and the record pointer. The old unit-local `VoiceAdpcmState` name is
retired rather than retained as another interchangeable owner.

The shared header and accepted clear operation now use the actual pointer.
No pointer-to-integer conversion, overlay, alias or new owner is introduced.

## RecordState +0x10 uses signed byte storage

The original census in `EVIDENCE.json` already records a signed one-byte read
at `0x001d914c` in `func_001d911c`, at record +0x10. Therefore the single shared
`loop` member is `signed char`, not the table's previously chosen `uint8_t`.
Other unsigned byte reads establish storage/truncation, not a second view.
The initializer still clears the same four distinct status bytes at +0x10.

## Native envelope status is an actual shared byte

The channel release consumers read and write their envelope status at +0x98
as one byte (`0x0017db00`/`0x0017db2c` and
`0x001d9544`/`0x001d9570`). The actual accepted reset provider is native
`nw::snd::internal::CurveAdshr::Reset`. The target declaration now has one
byte `m_Status` member, shared directly with those release consumers; its
public enum API performs an ordinary value conversion. There is no separate
`AudioEnvelope` owner or prefix cast. The native envelope unit and all other
changed-header dependents are qualified under their original frozen profiles.

## Admission boundary

The amendment does not close the separately unsealed complete SoundArchive
and hook producers, native null-deletion-slot/key-function ownership,
SequenceTrack/player/decoder families or forwarding FileStream protocol.
Their accepted models and deferred identities remain explicitly carried.
