"""Public catalogue-writer regressions over hand-encoded synthetic instructions only."""
from __future__ import annotations

import struct
import unittest

from tools import analyze
from tests.test_analyze import BASE, branch, fixture

SETEND_BE = 0xF1010200
SETEND_LE = 0xF1010000
NOP = 0xE320F000

# Exact sealed V10R4-R01 public counterexample; no original executable payload.
SEALED_ENDIAN_WORDS = [
    0xE92D4070, SETEND_BE, 0xE59F4014, 0xE5D45000, SETEND_LE, 0xE8BD8070,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, BASE + 44, 0xFFFFFFFF, 0x01010101, 0xFFFFFFFF,
]


def arm_reader(prefix: list[int], *, between: list[int] | None = None,
               tail: list[int] | None = None, displacement: int = 0,
               payload_words: int = 1) -> tuple[list[int], int, int]:
    """A saved-register reader with a real pool and never-decoded candidate data."""
    words = [0xE92D4070, *prefix]
    load = len(words) * 4
    words.extend([0, *(between or [])])
    read = len(words) * 4
    words.extend([0xE5D45000, *(tail or [SETEND_LE]), 0xE8BD8070])
    words.extend([0xFFFFFFFF] * 3)
    pool = len(words) * 4
    target = BASE + pool + 8
    words[load // 4] = 0xE59F4000 | (pool + displacement - load - 8)
    words.extend([target, 0xFFFFFFFF, *([0x01010101] * payload_words), 0xFFFFFFFF])
    return words, target, BASE + read


def thumb_reader(prefix: list[int], *, between: list[int] | None = None,
                 relative: bool = False) -> tuple[list[int], int, int]:
    """A BLX-rooted Thumb reader; halfwords are authored independently."""
    halfwords = [0xB570, *prefix]
    load = 16 + len(halfwords) * 2
    halfwords.append(0)
    halfwords.extend(between or [])
    if relative:
        halfwords.append(0x447C)  # add r4, pc
    read = 16 + len(halfwords) * 2
    halfwords.extend([0x7825, 0xB650, 0xBD70])  # ldrb r5, [r4]; SETEND LE; pop
    if len(halfwords) % 2:
        halfwords.append(0xFFFF)
    pool = 16 + len(halfwords) * 2
    target = BASE + pool + 8
    halfwords[(load - 16) // 2] = 0x4C00 | ((pool - ((load + 4) & ~3)) // 4)
    value = target - (load + 6 + 2 * len(between or [])) - BASE if relative else target
    body = struct.pack(f"<{len(halfwords)}H", *halfwords)
    words = [0xFA000002, 0xE12FFF1E, 0xFFFFFFFF, 0xFFFFFFFF,
             *struct.unpack(f"<{len(body) // 4}I", body), value & 0xFFFFFFFF,
             0xFFFFFFFF, 0x01010101, 0xFFFFFFFF]
    return words, target, BASE + read


class CatalogueEndianTests(unittest.TestCase):
    def assert_writer(self, words: list[int], target: int, site: int, *, claimed: bool,
                      width: int = 1, owner: int | None = None, **kwargs) -> dict:
        image, layout = fixture(words, **kwargs)
        result = analyze.build_catalogue(image, layout, closure={})
        # Assert the physical consumer, not just a helper's private result.
        row = next(row for row in result["ranges"] if row["start"] <= target < row["end"])
        self.assertEqual((row["kind"], row["owner"]),
                         ("function" if claimed and owner is not None else "rodata" if claimed else "unknown", owner))
        reads = [row for row in result["closure_evidence"]["read_footprints"] if row["site"] == site]
        if owner is None:
            self.assertEqual([(row["start"], row["end"]) for row in reads], [(target, target + width)] if claimed else [])
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])
        self.assertTrue(any(first <= site < end for function in result["functions"]
                            for first, end in function["code_ranges"]))
        return result

    def test_sealed_big_endian_pool_pointer_never_relabels_the_unread_byte(self) -> None:
        result = self.assert_writer(SEALED_ENDIAN_WORDS, BASE + 44, BASE + 12, claimed=False)
        self.assertTrue(any(row["site"] == BASE + 8 and row["evidence"] == "unproved_data_endian_pool_value_veto_only"
                            for row in result["closure_evidence"]["read_vetoes"]))

    def test_little_endian_entry_and_explicit_restoration_admit_new_arm_loads(self) -> None:
        for prefix in ([], [NOP], [SETEND_LE], [SETEND_BE, SETEND_LE], [SETEND_BE, SETEND_LE, SETEND_BE, SETEND_LE]):
            with self.subTest(prefix=prefix):
                words, target, site = arm_reader(prefix)
                self.assert_writer(words, target, site, claimed=True)

    def test_a_later_restoration_cannot_retroactively_validate_an_arm_pool_value(self) -> None:
        for between in ([], [SETEND_LE], [SETEND_LE, NOP]):
            with self.subTest(between=between):
                words, target, site = arm_reader([SETEND_BE], between=between)
                self.assert_writer(words, target, site, claimed=False)

    def test_one_changed_predecessor_at_a_join_remains_unknown_until_restored(self) -> None:
        for restored in (False, True):
            prefix = [0xE3500000, branch(BASE + 8, BASE + 16, condition=1), SETEND_BE]
            if restored:
                prefix.append(SETEND_LE)
            with self.subTest(restored=restored):
                words, target, site = arm_reader(prefix)
                self.assert_writer(words, target, site, claimed=restored)

    def test_a_big_endian_backedge_is_not_an_abi_entry_reset(self) -> None:
        for restored in (False, True):
            prefix = [SETEND_LE] if restored else []
            # Reachable BE path loops back through the owner's public entry.
            tail = [SETEND_BE, 0xE3500000, branch(BASE + 20 + len(prefix) * 4, BASE, condition=1), SETEND_LE]
            with self.subTest(restored=restored):
                words, target, site = arm_reader(prefix, tail=tail)
                self.assert_writer(words, target, site, claimed=restored)

    def test_a_cross_owner_big_endian_tail_does_not_reset_the_target_entry(self) -> None:
        for restored in (False, True):
            words, target, site = arm_reader([SETEND_LE] if restored else [])
            # A second primary-rooted caller enters BE then tails to the reader.
            caller = BASE + len(words) * 4
            words.extend([0xE92D4070, SETEND_BE, branch(caller + 8, BASE), 0xFFFFFFFF])
            with self.subTest(restored=restored):
                self.assert_writer(words, target, site, claimed=restored)

    def test_secondary_entry_after_restoration_has_no_independent_le_proof(self) -> None:
        for entry_before_restoration in (False, True):
            prefix = [SETEND_BE]
            # A genuine BL target prologue inside an otherwise falling-through owner.
            if not entry_before_restoration:
                prefix.append(SETEND_LE)
            entry = BASE + 4 + 4 * len(prefix)
            prefix.append(0xE92D4070)
            if entry_before_restoration:
                prefix.append(SETEND_LE)
            words, target, site = arm_reader(prefix)
            caller = BASE + len(words) * 4
            words.extend([0xE92D4010, branch(caller + 4, entry, link=True), 0xE8BD8010])
            with self.subTest(entry_before_restoration=entry_before_restoration):
                result = self.assert_writer(words, target, site, claimed=entry_before_restoration)
                self.assertTrue(any(item["address"] == entry for function in result["functions"]
                                    for item in function["entries"]))

    def test_ordinary_call_cannot_restore_unproved_state_but_preserves_proven_le(self) -> None:
        for big_endian in (False, True):
            for condition in (1, 14):
                words, target, site = arm_reader([SETEND_BE] if big_endian else [], between=[0, 0xE3500000])
                call = BASE + 8 + (4 if big_endian else 0)
                callee = BASE + len(words) * 4
                words[(call - BASE) // 4] = branch(call, callee, link=True, condition=condition)
                words.extend([0xE3A00000, 0xE12FFF1E])
                with self.subTest(big_endian=big_endian, condition=condition):
                    self.assert_writer(words, target, site, claimed=not big_endian)

    def test_processor_status_changes_never_seed_later_pool_values(self) -> None:
        states = (0xE122F000, 0xE321F001, 0xEE010F10, 0xF10C0080, 0xE1B0F00E, 0xE8FD8010,
                  0xF8BD0A00, 0xF83D0A00, 0xF93D0A00, 0xF9BD0A00)
        for state in states:
            for condition in (0, 1, 14):
                # A conditional branch retains the reader in the owned CFG even
                # when the unmodeled opcode transfers. SETEND LE cannot erase the
                # unsupported owner effect/proof obligation.
                instruction = state if state >> 28 == 15 else state & 0x0FFFFFFF | condition << 28
                prefix = [0xE3500000, branch(BASE + 8, BASE + 20, condition=1), instruction, SETEND_LE]
                words, target, site = arm_reader(prefix)
                with self.subTest(state=state, condition=condition):
                    self.assert_writer(words, target, site, claimed=False)

    def test_unreachable_processor_change_does_not_supply_an_owner_state_proof(self) -> None:
        words, target, site = arm_reader([branch(BASE + 4, BASE + 16), 0xE122F000, SETEND_LE])
        # A real call roots the otherwise skipped status-changing entry in the
        # owner's code inventory; no positive proof follows from lexical order.
        caller = BASE + len(words) * 4
        words.extend([0xE92D4010, branch(caller + 4, BASE + 8, link=True), 0xE8BD8010])
        self.assert_writer(words, target, site, claimed=False)

    def test_thumb_pool_values_follow_endian_state_and_not_decoder_constants(self) -> None:
        for prefix, claimed in (([], True), ([0xB650], True), ([0xB658], False), ([0xB658, 0xB650], True)):
            for relative in (False, True):
                with self.subTest(prefix=prefix, relative=relative):
                    words, target, site = thumb_reader(prefix, relative=relative)
                    self.assert_writer(words, target, site, claimed=claimed,
                                       owner=BASE + 16 if relative and claimed else None)
        words, target, site = thumb_reader([0xB658], between=[0xB650])
        self.assert_writer(words, target, site, claimed=False)

    def test_thumb_pool_derived_tagged_code_addresses_require_le_before_the_load(self) -> None:
        for state, claimed in ((0xB650, True), (0xB658, False)):
            body = struct.pack("<8H", 0xB510, state, 0x4C02, 0x447C, 0xB650, 0xBD10, 0xFFFF, 0xFFFF)
            # Put the callback before its reader so the existing data-object
            # ownership guards cannot absorb an as-yet-unproved forward body.
            target = BASE + 16
            constant = (target + 1 - (BASE + 38 + 4)) & 0xFFFFFFFF
            words = [0xFA000006, 0xE12FFF1E, 0xFFFFFFFF, 0xFFFFFFFF,
                     0x47702007, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     *struct.unpack("<4I", body), constant, 0xFFFFFFFF]
            image, layout = fixture(words)
            result = analyze.build_catalogue(image, layout, closure={})
            witnesses = [row for row in result["pc_relative_pointer_witnesses"] if row["target"] == target]
            with self.subTest(state=state):
                self.assertEqual(bool(witnesses), claimed)
                self.assertEqual(any(function["address"] == target for function in result["functions"]), claimed)
                self.assertEqual(analyze.validate_catalogue(result, image, layout), [])

    def test_thumb_it_and_status_writes_are_not_inferred_harmless_from_missing_effects(self) -> None:
        for state in ([0xBF18, 0xB658], [0xF380, 0x8200], [0xB672]):
            with self.subTest(state=state):
                words, target, site = thumb_reader(state)
                self.assert_writer(words, target, site, claimed=False)

    def test_direct_pc_byte_reads_are_endian_independent_and_keep_exact_width(self) -> None:
        words, target, site = arm_reader([SETEND_BE])
        words[(site - BASE) // 4] = 0xE5DF5000 | (target - site - 8)
        self.assert_writer(words, target, site, claimed=True, owner=BASE)

    def test_conditional_reads_remain_veto_only_after_a_proven_le_load(self) -> None:
        words, target, site = arm_reader([])
        words[(site - BASE) // 4] = 0x15D45000
        result = self.assert_writer(words, target, site, claimed=False)
        self.assertTrue(any(row["site"] == site and row["evidence"] == "conditional_read_veto_only"
                            for row in result["closure_evidence"]["read_vetoes"]))

    def test_unaligned_pool_addresses_do_not_reuse_a_rounded_word_value(self) -> None:
        for displacement in (1, 2, 3):
            with self.subTest(displacement=displacement):
                words, target, site = arm_reader([], displacement=displacement)
                self.assert_writer(words, target, site, claimed=False)

    def test_endian_refusal_keeps_alignment_veto_and_anomaly_visible(self) -> None:
        words = [SETEND_BE, 0xE59F0018, 0xE1D010B0, SETEND_LE, 0xFA000000, 0xE12FFF1E,
                 0x47702001, 0x0000FFFF, BASE + 30, 0xFFFFFFFF]
        image, layout = fixture(words)
        result = analyze.build_catalogue(image, layout, closure={})
        row = next(row for row in result["ranges"] if row["start"] <= BASE + 30 < row["end"])
        self.assertEqual((row["kind"], row["owner"]), ("unknown", None))
        self.assertFalse(any(item["start"] <= BASE + 30 < item["end"]
                             for item in result["closure_evidence"]["alignment"]))
        self.assertTrue(any(item["evidence"] == "unproved_data_endian_pool_value_veto_only"
                            for item in result["closure_evidence"]["read_vetoes"]))
        self.assertEqual(analyze.validate_catalogue(result, image, layout), [])

    def test_big_endian_veneer_ingress_cannot_reset_the_real_reader_entry(self) -> None:
        for restored in (False, True):
            words, target, site = arm_reader([SETEND_LE] if restored else [])
            veneer = BASE + len(words) * 4
            caller = veneer + 12
            words.extend([0xE51FF004, BASE, 0xFFFFFFFF, 0xE92D4070,
                          SETEND_BE, branch(caller + 8, veneer), 0xFFFFFFFF])
            with self.subTest(restored=restored):
                result = self.assert_writer(words, target, site, claimed=restored)
                self.assertTrue(any(function["address"] == veneer and function["kind"] == "veneer"
                                    for function in result["functions"]))

    def test_repaired_service_outputs_still_invalidate_saved_pointer_values(self) -> None:
        for register in (4, 5):
            for condition in (1, 14):
                words = [0xE92D4070, 0xE3A02000, 0xE59F000C | register << 12, 0xE3520001,
                         condition << 28 | 0x0F000002, 0xE1D060F0 | register << 16, 0xE8BD8070,
                         BASE + 40, 0xFFFFFFFF, 0xFFFFFFFF, 0x01010101, 0xFFFFFFFF]
                with self.subTest(register=register, condition=condition):
                    self.assert_writer(words, BASE + 40, BASE + 20, claimed=False, width=2)

    def test_supported_multi_register_loads_do_not_taint_a_fresh_le_pool_value(self) -> None:
        for instruction in (0xE8900003, 0xE1C200D0, 0xECBD0B04, 0xEEF10A10, 0xE3500000):
            with self.subTest(instruction=instruction):
                words, target, site = arm_reader([instruction])
                self.assert_writer(words, target, site, claimed=True)

    def test_setend_after_a_conditional_branch_is_not_reset_by_lexical_order(self) -> None:
        for restored in (False, True):
            prefix = [0xE3500000, branch(BASE + 8, BASE + 20, condition=1), SETEND_BE,
                      branch(BASE + 16, BASE + 24), NOP]
            if restored:
                prefix.append(SETEND_LE)
            words, target, site = arm_reader(prefix)
            with self.subTest(restored=restored):
                self.assert_writer(words, target, site, claimed=restored)


if __name__ == "__main__":
    unittest.main()
