"""Exact runtime-section data and exceptional continuation closure.

Frozen metadata authenticates the licensed objects. Generation reads only the
original image: full-section/partition digests, independent code references and
an all-path descriptor contract are re-derived without scanning vendor archives.
"""
from __future__ import annotations

from dataclasses import dataclass
import hashlib
import itertools
import json
from pathlib import Path
from collections import Counter

import capstone
from capstone import arm

MASK = (1 << 32) - 1
FLAGS = frozenset(itertools.product((0, 1), repeat=4))


def _read(analyzer, start: int, end: int) -> bytes:
    if not analyzer.base <= start < end <= analyzer.end:
        raise ValueError("runtime section is outside the bounded original text")
    return analyzer.text[start - analyzer.base:end - analyzer.base]


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _record_sha(record: dict) -> str:
    return _sha((json.dumps(record, indent=2, sort_keys=True, ensure_ascii=False) + "\n").encode())


@dataclass(frozen=True)
class Value:
    values: frozenset[int] | None
    lower: int = 0
    upper: int = MASK

    @classmethod
    def finite(cls, values) -> Value:
        values = frozenset(value & MASK for value in values)
        return cls(values, min(values), max(values))

    @classmethod
    def interval(cls, lower: int, upper: int) -> Value:
        if not 0 <= lower <= upper <= MASK:
            return cls(None)
        if upper - lower <= 32:
            return cls.finite(range(lower, upper + 1))
        return cls(None, lower, upper)


UNKNOWN = Value(None)


def shifted(value: Value, kind: int, amount: Value) -> Value:
    def shift(v: int, n: int) -> int:
        if kind == arm.ARM_SFT_LSL:
            return (v << n) & MASK if n < 32 else 0
        if kind == arm.ARM_SFT_LSR:
            return v >> n if n < 32 else 0
        raise ValueError("unsupported descriptor shift")

    if value.values is not None and amount.values is not None:
        return Value.finite(shift(v, n) for v in value.values for n in amount.values)
    if amount.values is not None and len(amount.values) == 1:
        n, = amount.values
        if kind == arm.ARM_SFT_LSR:
            return Value.interval(value.lower >> n, value.upper >> n) if n < 32 else Value.finite((0,))
        if kind == arm.ARM_SFT_LSL:
            return Value.interval(value.lower << n, value.upper << n) if n < 32 else Value.finite((0,))
    return UNKNOWN


def operand_value(op, registers: dict[int, Value]) -> Value:
    if op.type == arm.ARM_OP_REG:
        value = registers.get(op.reg, UNKNOWN)
    elif op.type == arm.ARM_OP_IMM:
        value = Value.finite((op.imm,))
    else:
        raise ValueError("non-scalar descriptor operand")
    kind, amount = op.shift.type, op.shift.value
    if kind in (arm.ARM_SFT_LSL_REG, arm.ARM_SFT_LSR_REG):
        base_kind = arm.ARM_SFT_LSL if kind == arm.ARM_SFT_LSL_REG else arm.ARM_SFT_LSR
        amount_value = registers.get(amount, UNKNOWN)
        amount_value = (Value.finite(v & 0xFF for v in amount_value.values)
                        if amount_value.values is not None else Value.interval(0, 255))
        return shifted(value, base_kind, amount_value)
    if kind:
        return shifted(value, kind, Value.finite((amount,)))
    return value


def concrete_operand(op, assignments: dict[int, int]) -> int:
    value = assignments[op.reg] if op.type == arm.ARM_OP_REG else op.imm & MASK
    kind, amount = op.shift.type, op.shift.value
    if kind in (arm.ARM_SFT_LSL_REG, arm.ARM_SFT_LSR_REG):
        amount = assignments[amount] & 0xFF
        kind = arm.ARM_SFT_LSL if kind == arm.ARM_SFT_LSL_REG else arm.ARM_SFT_LSR
    if kind == arm.ARM_SFT_LSL:
        value = (value << amount) & MASK if amount < 32 else 0
    elif kind == arm.ARM_SFT_LSR:
        value = value >> amount if amount < 32 else 0
    elif kind:
        raise ValueError("unsupported descriptor scalar shift")
    return value


def result_flags(left: int, right: int, operation: int, carry: int = 0) -> tuple[int, int, int, int]:
    if operation in (arm.ARM_INS_ADD, arm.ARM_INS_ADC):
        wide = left + right + carry
        result = wide & MASK
        c = int(wide > MASK)
        v = int(bool((~(left ^ right) & (left ^ result)) & (1 << 31)))
    elif operation in (arm.ARM_INS_SUB, arm.ARM_INS_CMP):
        result = (left - right) & MASK
        c = int(left >= right)
        v = int(bool(((left ^ right) & (left ^ result)) & (1 << 31)))
    else:
        raise ValueError("unsupported descriptor flag effect")
    return result >> 31, int(result == 0), c, v


def condition_holds(cc: int, flags: tuple[int, int, int, int]) -> bool:
    n, z, c, v = flags
    conditions = {
        arm.ARM_CC_AL: True, arm.ARM_CC_EQ: bool(z), arm.ARM_CC_NE: not z,
        arm.ARM_CC_MI: bool(n), arm.ARM_CC_PL: not n,
        arm.ARM_CC_HS: bool(c), arm.ARM_CC_LO: not c,
        arm.ARM_CC_HI: bool(c and not z), arm.ARM_CC_LS: not c or bool(z),
    }
    if cc not in conditions:
        raise ValueError("unsupported descriptor condition")
    return conditions[cc]


def binary_value(ins, source_ops, registers: dict[int, Value], flags) -> tuple[Value, frozenset]:
    source_registers = {op.reg for op in source_ops if op.type == arm.ARM_OP_REG}
    source_registers.update(op.shift.value for op in source_ops
                            if op.shift.type in (arm.ARM_SFT_LSL_REG, arm.ARM_SFT_LSR_REG))
    finite = all(registers.get(reg, UNKNOWN).values is not None for reg in source_registers)
    carries = {flag[2] for flag in flags} if ins.id == arm.ARM_INS_ADC else {0}
    if finite:
        ids = sorted(source_registers)
        combinations = 1
        for reg in ids:
            combinations *= len(registers[reg].values)
        if combinations <= 4096:
            values, new_flags = set(), set()
            for choice in itertools.product(*(registers[reg].values for reg in ids)):
                assignments = dict(zip(ids, choice))
                left, right = (concrete_operand(op, assignments) for op in source_ops)
                for carry in carries:
                    if ins.id in (arm.ARM_INS_ADD, arm.ARM_INS_ADC):
                        value = left + right + carry
                    elif ins.id == arm.ARM_INS_SUB:
                        value = left - right
                    elif ins.id == arm.ARM_INS_AND:
                        value = left & right
                    else:
                        raise ValueError("unsupported descriptor integer operation")
                    values.add(value & MASK)
                    if ins.update_flags:
                        new_flags.add(result_flags(left, right, ins.id, carry))
            return Value.finite(values), frozenset(new_flags) if ins.update_flags else flags
    left, right = (operand_value(op, registers) for op in source_ops)
    if ins.id in (arm.ARM_INS_ADD, arm.ARM_INS_ADC):
        value = Value.interval(left.lower + right.lower + min(carries), left.upper + right.upper + max(carries))
    elif ins.id == arm.ARM_INS_SUB:
        value = Value.interval(left.lower - right.upper, left.upper - right.lower)
    elif ins.id == arm.ARM_INS_AND and right.values is not None and len(right.values) == 1:
        maximum, = right.values
        value = Value.interval(0, maximum)
    else:
        value = UNKNOWN
    return value, FLAGS if ins.update_flags else flags


def descriptor_paths(analyzer, start: int, size: int, descriptor: int) -> dict:
    """Sound finite-set/unsigned-interval overapproximation of every incoming register.

    The only modeled memory is one LR-addressed, post-incremented 32-bit descriptor
    and restoration of the caller's saved r4/LR pair. The public little-endian ARM
    ABI is required at the bound entry; the caller-prefix and this interpreter
    admit no SETEND, status/state restoration or unknown system effect. Unknown
    transfers/effects, loops, extra reads, descriptor-selected internal dispatch
    and unproved returns refuse this intentionally small contract.
    """
    decoder = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    decoder.detail = True
    instructions = {ins.address: ins for ins in decoder.disasm(_read(analyzer, start, start + size), start)}
    if list(instructions) != list(range(start, start + size, 4)):
        raise ValueError("incomplete descriptor dispatcher section")
    registers = {reg: UNKNOWN for reg in range(arm.ARM_REG_R0, arm.ARM_REG_R12 + 1)}
    registers[arm.ARM_REG_LR] = Value.finite((descriptor,))
    queue = [(start, registers, FLAGS, False, False, frozenset())]
    targets, read_sites, restore_sites, transfer_sites, visited = set(), set(), set(), set(), set()
    categories = set()
    terminal_paths = processed = 0
    while queue:
        processed += 1
        if processed > 4096:
            raise ValueError("descriptor complete-path bound exceeded")
        site, registers, flags, read, restored, history = queue.pop()
        if site in history or site not in instructions:
            raise ValueError("unbounded or escaping descriptor control flow")
        if len(history) > size // 4:
            raise ValueError("descriptor path exceeds section instruction bound")
        ins = instructions[site]
        visited.add(site)
        yes = frozenset(flag for flag in flags if condition_holds(ins.cc, flag))
        no = flags - yes
        if no:
            queue.append((site + 4, dict(registers), no, read, restored, history | {site}))
        if not yes:
            continue
        state = dict(registers)
        next_site, next_flags, next_read, next_restored = site + 4, yes, read, restored
        ops = ins.operands
        if ins.id == arm.ARM_INS_B:
            if len(ops) != 1 or ops[0].type != arm.ARM_OP_IMM:
                raise ValueError("opaque descriptor branch")
            next_site = ops[0].imm
        elif ins.id == arm.ARM_INS_LDR:
            if (read or len(ops) != 3 or ops[0].type != arm.ARM_OP_REG
                    or ops[0].reg != arm.ARM_REG_R4 or ops[1].type != arm.ARM_OP_MEM
                    or ops[1].mem.base != arm.ARM_REG_LR or ops[1].mem.index
                    or ops[1].mem.disp or ops[2].type != arm.ARM_OP_IMM
                    or ops[2].imm != 4 or not ins.writeback or ins.update_flags
                    or ins.usermode or ins.cc != arm.ARM_CC_AL
                    or state[arm.ARM_REG_LR].values != frozenset((descriptor,))):
                raise ValueError("descriptor has no exact single post-LR word read")
            state[arm.ARM_REG_R4] = Value.finite((int.from_bytes(_read(analyzer, descriptor, descriptor + 4), "little"),))
            state[arm.ARM_REG_LR] = Value.finite((descriptor + 4,))
            read_sites.add(site)
            next_read = True
        elif ins.id == arm.ARM_INS_TST:
            if len(ops) != 2 or any(op.type != arm.ARM_OP_REG or op.shift.type for op in ops):
                raise ValueError("descriptor test has an unmodeled carry-shifter effect")
            left, right = (operand_value(op, state) for op in ops)
            if left.values is not None and right.values is not None:
                next_flags = frozenset((value >> 31, int(value == 0), c, v)
                                      for a in left.values for b in right.values
                                      for value in (a & b,) for _, _, c, v in yes)
            else:
                next_flags = FLAGS
        elif ins.id == arm.ARM_INS_CMP:
            left, right = (operand_value(op, state) for op in ops)
            next_flags = (frozenset(result_flags(a, b, arm.ARM_INS_CMP)
                                   for a in left.values for b in right.values)
                          if left.values is not None and right.values is not None else FLAGS)
        elif ins.id in (arm.ARM_INS_MOV, arm.ARM_INS_LSL, arm.ARM_INS_LSR):
            if (ins.update_flags or ins.writeback or not ops or ops[0].type != arm.ARM_OP_REG
                    or ops[0].reg in (arm.ARM_REG_LR, arm.ARM_REG_SP, arm.ARM_REG_PC)):
                raise ValueError("unmodeled descriptor scalar flag effect")
            if len(ops) == 2:
                value = operand_value(ops[1], state)
            elif len(ops) == 3 and ins.id in (arm.ARM_INS_LSL, arm.ARM_INS_LSR):
                value = shifted(operand_value(ops[1], state),
                                arm.ARM_SFT_LSL if ins.id == arm.ARM_INS_LSL else arm.ARM_SFT_LSR,
                                (Value.finite(v & 0xFF for v in operand_value(ops[2], state).values)
                                 if operand_value(ops[2], state).values is not None else Value.interval(0, 255))
                                if ops[2].type == arm.ARM_OP_REG else operand_value(ops[2], state))
            else:
                raise ValueError("unmodeled descriptor scalar shape")
            state[ops[0].reg] = value
        elif ins.id in (arm.ARM_INS_ADD, arm.ARM_INS_ADC, arm.ARM_INS_SUB, arm.ARM_INS_AND):
            if (len(ops) != 3 or ops[0].type != arm.ARM_OP_REG or ins.writeback
                    or ops[0].reg in (arm.ARM_REG_LR, arm.ARM_REG_SP, arm.ARM_REG_PC)):
                raise ValueError("descriptor requires an unsupported computed internal transfer")
            value, next_flags = binary_value(ins, ops[1:], state, yes)
            if ops[0].reg == arm.ARM_REG_R3 and any(op.type == arm.ARM_OP_REG and op.reg == arm.ARM_REG_R12 for op in ops[1:]):
                category = state[arm.ARM_REG_R12]
                if category.values is None or not category.values <= frozenset(range(9)):
                    raise ValueError("descriptor category is not bounded by the complete dispatcher")
                categories.update(category.values)
            state[ops[0].reg] = value
        elif ins.id == arm.ARM_INS_POP:
            if (ins.usermode or ins.cc != arm.ARM_CC_AL
                    or [op.reg for op in ops] != [arm.ARM_REG_R4, arm.ARM_REG_LR] or restored or not read):
                raise ValueError("descriptor lacks exact saved-pair stack restoration")
            state[arm.ARM_REG_R4] = state[arm.ARM_REG_LR] = UNKNOWN
            restore_sites.add(site)
            next_restored = True
        elif ins.id == arm.ARM_INS_BX:
            if (len(ops) != 1 or ops[0].type != arm.ARM_OP_REG or not read or not restored
                    or state.get(ops[0].reg, UNKNOWN).values is None):
                raise ValueError("descriptor continuation is not exact on every path")
            destinations = state[ops[0].reg].values
            if not destinations or not destinations <= frozenset(descriptor + 4 + 4 * index for index in range(4)):
                raise ValueError("descriptor continuation escapes its four explicit slots")
            targets.update(destinations)
            transfer_sites.add(site)
            terminal_paths += 1
            continue
        else:
            raise ValueError("unsupported descriptor effect")
        queue.append((next_site, state, next_flags, next_read, next_restored, history | {site}))
        if len(queue) > 4096:
            raise ValueError("descriptor abstract state bound exceeded")
    if not terminal_paths or len(read_sites) != 1 or len(restore_sites) != 1 or len(transfer_sites) != 1:
        raise ValueError("descriptor paths do not close a single exact continuation contract")
    return {
        "dispatcher": start, "dispatcher_size": size, "descriptor": descriptor,
        "descriptor_width": 4, "return_address_delta": 4,
        "all_incoming_core_register_values": True, "abstract_domain": "finite sets / sound unsigned intervals",
        "conditional_flags": "all possible incoming NZCV; exact where modeled finite operands allow",
        "entry_data_endian": "little", "unmodeled_state_or_endian_effects": "refused",
        "category_values": sorted(categories), "continuation_targets": sorted(targets),
        "descriptor_read_sites": sorted(read_sites), "saved_pair_restore_sites": sorted(restore_sites),
        "continuation_transfer_sites": sorted(transfer_sites), "visited_instruction_sites": sorted(visited),
        "terminal_abstract_paths": terminal_paths,
        "global_noreturn_inferred": False,
    }



def load_inputs(root: Path, parent_path: Path) -> tuple[dict, dict[str, str]]:
    """Authenticate a new proposal child without changing any historical input."""
    path = root / "research/catalogue_v11_evidence.json"
    payload = path.read_bytes()
    evidence = json.loads(payload)
    parent = evidence["parent_evidence"]
    if (root / parent["path"] != parent_path or _sha(parent_path.read_bytes()) != parent["sha256"]
            or evidence["image_sha256"] != json.loads(parent_path.read_bytes())["image_sha256"]):
        raise ValueError("v11 runtime evidence differs from its immutable parent/image")
    prior = evidence["prior_proposal"]
    if (len(prior["sha256"]) != 64 or len(prior["commit"]) != 40
            or prior["unknown_bytes"] <= 0 or prior["unknown_intervals"] <= 0
            or prior["adopted"] or evidence["new_C_credit"] != 0):
        raise ValueError("v11 prior proposal cannot become adoption or a zero-unknown assertion")
    ids = [record["id"] for record in evidence["sections"]]
    if len(ids) != len(set(ids)):
        raise ValueError("duplicate exact runtime-section witness")
    for record in evidence["sections"]:
        section = record["section"]
        witness = record["object_witness"]
        bound = record["binding"]
        if (not section["allocated"] or not section["executable"] or section["writable"]
                or section["size"] <= 0 or section["size"] > 0x10000 or section["alignment"] not in (2, 4, 8, 16)
                or bound["isa"] != "arm" or bound["address"] % section["alignment"]
                or bound["symbol_offset"] != 0 or not bound["incoming_sources"]
                or witness["exact_build"] != "5.04-b82" or not witness["whole_section_reencoded_equal"]
                or witness["image_section_sha256"] != section["image_sha256"]
                or not witness["archive_member"] or witness["member_offset"] < 8):
            raise ValueError("runtime closure needs a complete independently bound exact-build section")
        symbols = section["symbols"]
        if (not any(symbol["name"] == bound["symbol"] and symbol["kind"] == "STT_FUNC"
                    and symbol["offset"] == bound["symbol_offset"] for symbol in symbols)
                or not any(symbol["kind"] == "STT_SECTION" and symbol["offset"] == 0
                           and symbol["size"] == section["size"] for symbol in symbols)
                or any(symbol["kind"] == "STT_FUNC" and symbol["offset"] + symbol["size"] > section["size"]
                       for symbol in symbols)):
            raise ValueError("runtime closure lacks real function/complete-section declarations")
        for digest in (section["image_sha256"], section["object_sha256"], witness["member_sha256"],
                       witness["archive_sha256"], bound["prior_record_sha256"]):
            if (not isinstance(digest, str) or len(digest) != 64
                    or any(character not in "0123456789abcdef" for character in digest)):
                raise ValueError("runtime witness contains an invalid digest")
        source = evidence["retained_source_rows"][str(witness["source_match_index"])]
        match, check = source["match"], source["relocation_check"]
        if (match["name"] != bound["symbol"] or match["size"] != section["size"]
                or match["object_body_sha256"] != section["object_sha256"]
                or match["hit"]["image_body_sha256"] != section["image_sha256"]
                or match["object_section"] != section["index"] or match["object_offset"] != bound["symbol_offset"]
                or not check["whole_relocated_body_equal"] or check["size"] != section["size"]
                or check["address"] != bound["address"] or check["symbol"] != bound["symbol"]
                or check["relocated_body_sha256"] != section["image_sha256"]
                or match["member"] != witness["archive_member"]
                or match["member_sha256"] != witness["member_sha256"]
                or not match["hit"]["whole_body_masked_equal"]):
            raise ValueError("runtime whole section is not the retained complete relocated object")
        fields = {(relocation["offset"], relocation["type"], relocation["symbol"])
                  for relocation in match["relocation_fields"]}
        if fields != {(relocation["offset"], relocation["type"], relocation["symbol"])
                      for relocation in section["relocations"]}:
            raise ValueError("runtime closure omits or adds an original object relocation")
        original_relocations = {(row["offset"], row["type"], row["symbol"]): row for row in check["relocations"]}
        for relocation in section["relocations"]:
            original = original_relocations.get((relocation["offset"], relocation["type"], relocation["symbol"]), {})
            if relocation["type"] not in (28, 40):
                raise ValueError("runtime section contains an unsupported relocation")
            if relocation["type"] == 28 and (not relocation["reencoded_equal"]
                    or original.get("target_address") != relocation["target"]):
                raise ValueError("runtime call relocation has no full original reencoding proof")
        mapping = section["mapping"]
        partitions = section["partitions"]
        if (not mapping or mapping[0] != [0, "$a"] or not partitions
                or partitions[0]["offset"] != 0 or partitions[-1]["end_offset"] != section["size"]
                or [[part["offset"], "$a" if part["kind"] == "code" else "$d"] for part in partitions] != mapping
                or any(part["kind"] not in ("code", "data") or part["offset"] >= part["end_offset"]
                       for part in partitions)
                or any(part["offset"] % 4 or part["end_offset"] % 4 for part in partitions)
                or any(left["end_offset"] != right["offset"] for left, right in zip(partitions, partitions[1:]))):
            raise ValueError("runtime section lacks complete, gap-free code/data mapping bounds")
        if record["closure_kind"] not in ("unowned_data", "descriptor_continuation"):
            raise ValueError("unsupported runtime closure kind")
        if record["closure_kind"] == "descriptor_continuation":
            historical = record["prior_function"]["classification_evidence"]
            if (not any(symbol["kind"] == "STT_FUNC" and symbol["offset"] == 0
                        and symbol["size"] == section["size"] for symbol in symbols)
                    or [part["kind"] for part in partitions] != ["code", "data", "code"]
                    or partitions[0]["end_offset"] != record["prior_function"]["size"]
                    or partitions[1]["end_offset"] - partitions[1]["offset"] != 4):
                raise ValueError("runtime descriptor requires a complete exact declaration and single word")
            classification = record["classification_record"]
            if (record["prior_function"]["address"] != bound["address"]
                    or _record_sha(record["prior_function"]) != bound["prior_record_sha256"]
                    or classification["size"] != section["size"] or classification["isa"] != bound["isa"]
                    or classification["kind"] != "runtime" or classification["basis"] != "compiler_generated"
                    or classification["identity"] != historical["identity"]
                    or classification["original_extent_sha256"] != section["image_sha256"]
                    or classification["historical_classification_evidence"] != historical
                    or classification["image_sha256"] != evidence["image_sha256"]):
                raise ValueError("runtime new extent cannot borrow its historical shorter proof")
    inputs = {str(path.relative_to(root)): _sha(payload)}
    if "prepared_protection" in evidence:
        descriptor = evidence["prepared_protection"]
        protection_path = root / descriptor["path"]
        content = protection_path.read_bytes()
        if _sha(content) != descriptor["sha256"]:
            raise ValueError("v11 prepared identity snapshot differs from its frozen input")
        evidence["_prepared_protection"] = json.loads(content)
        inputs[descriptor["path"]] = descriptor["sha256"]
    return evidence, inputs


def _check_binding(catalogue: dict, analyzer, record: dict) -> None:
    functions = {function["address"]: function for function in catalogue["functions"]}
    binding = record["binding"]
    function = functions.get(binding["address"])
    if function is None or function["isa"] != binding["isa"]:
        raise ValueError("runtime section's original entry is absent")
    known_entries = {entry["address"] for entry in function["entries"]}
    if binding["address"] not in known_entries:
        raise ValueError("runtime section placement is not an existing entry")
    if record["closure_kind"] == "unowned_data":
        if _record_sha(function) != binding["prior_record_sha256"]:
            raise ValueError("runtime data closure changed a preserved function record")
    elif function["size"] == record["prior_function"]["size"]:
        if _record_sha(function) != binding["prior_record_sha256"]:
            raise ValueError("runtime extent closure is not bound to the complete original record")
    for site in binding["incoming_sources"]:
        owners = [owner for owner in catalogue["functions"]
                  if any(first <= site < end for first, end in owner["code_ranges"])]
        instruction = analyzer.decode(site, owners[0]["isa"]) if len(owners) == 1 else None
        if (instruction is None or not (instruction.call or instruction.branch)
                or instruction.target != (binding["address"], binding["isa"])):
            raise ValueError("runtime entry placement lacks its independently owned code reference")
    for reference in record.get("placement_references", []):
        caller = functions.get(reference["caller"])
        if (caller is None or caller["isa"] != reference["isa"] or caller["size"] != reference["caller_size"]
                or not any(first <= reference["site"] < end for first, end in caller["code_ranges"])
                or _sha(_read(analyzer, caller["address"], caller["address"] + caller["size"]))
                != reference["caller_image_sha256"]):
            raise ValueError("runtime placement's authenticated caller extent has changed")
        classification = caller.get("classification_evidence", {})
        if classification.get("identity") != reference["caller_identity"]:
            raise ValueError("runtime placement cannot borrow an unauthenticated caller identity")
        instruction = analyzer.decode(reference["site"], reference["isa"])
        if instruction is None or not instruction.call or instruction.target != (binding["address"], binding["isa"]):
            raise ValueError("runtime section reference does not bind the selected entry")


def _check_section(analyzer, record: dict) -> None:
    section = record["section"]
    base = record["binding"]["address"]
    if _sha(_read(analyzer, base, base + section["size"])) != section["image_sha256"]:
        raise ValueError("complete runtime section no longer equals the frozen relocated object")
    for partition in section["partitions"]:
        if (_sha(_read(analyzer, base + partition["offset"], base + partition["end_offset"]))
                != partition["image_sha256"]):
            raise ValueError("complete runtime code/data partition digest differs")
    for relocation in section["relocations"]:
        if relocation["type"] == 40:
            if relocation["agreement"] != "unchanged_marker":
                raise ValueError("runtime section has an unsupported relocation marker")
            continue
        if relocation["type"] != 28 or relocation["agreement"] not in (
                "authenticated_exact_callee", "observed_external_target_not_API_identity"):
            raise ValueError("runtime section relocation has no exact target closure")
        site = base + relocation["offset"]
        instruction = analyzer.decode(site, "arm")
        if instruction is None or not instruction.call or instruction.target != (relocation["target"], "arm"):
            raise ValueError("runtime relocated call target differs from its exact witness")
        if relocation["agreement"] == "authenticated_exact_callee":
            target = relocation["target"]
            if (_sha(_read(analyzer, target, target + relocation["target_size"]))
                    != relocation["target_image_sha256"]):
                raise ValueError("runtime descriptor callee is not the authenticated complete body")


def _check_data_reference(catalogue: dict, analyzer, record: dict) -> None:
    reference = record["data_reference"]
    function = next((function for function in catalogue["functions"]
                     if function["address"] == record["binding"]["address"]), None)
    if function is None or not all(any(first <= site < end for first, end in function["code_ranges"])
                                   for site in (reference["address_site"], reference["read_site"])):
        raise ValueError("runtime data has no independently owned address/read sites")
    address_instruction = analyzer.decode(reference["address_site"], "arm")
    if address_instruction is None or address_instruction.address_target != reference["address_base"]:
        raise ValueError("runtime data's original address-forming reference differs")
    read = analyzer.detailed(reference["read_site"])
    if (read is None or read.cc != arm.ARM_CC_AL or read.writeback
            or read.id != (arm.ARM_INS_LDRB if reference["width"] == 1 else arm.ARM_INS_LDR)):
        raise ValueError("runtime data has no witnessed scalar indexed read")
    memories = [operand for operand in read.operands if operand.type == arm.ARM_OP_MEM]
    if (len(memories) != 1 or not memories[0].mem.base or not memories[0].mem.index
            or memories[0].mem.disp or memories[0].shift.type != reference["shift_type"]
            or memories[0].shift.value != reference["shift_amount"]):
        raise ValueError("runtime indexed read is not the frozen address/width form")


def _replace_unknown_ranges(catalogue: dict, replacements: list[tuple[int, int, str, int | None]]) -> None:
    ranges = []
    for old in catalogue["ranges"]:
        cuts = {old["start"], old["end"]}
        applicable = [row for row in replacements if old["start"] < row[1] and row[0] < old["end"]]
        for first, end, _, _ in applicable:
            cuts.update((max(first, old["start"]), min(end, old["end"])))
        ordered = sorted(cuts)
        for first, end in zip(ordered, ordered[1:]):
            kind, owner = old["kind"], old["owner"]
            covering = [row for row in applicable if row[0] <= first < end <= row[1]]
            if covering:
                if len({(row[2], row[3]) for row in covering}) != 1:
                    raise ValueError("runtime witnesses have contradictory physical ownership")
                expected_kind, expected_owner = covering[0][2:]
                if kind == "unknown":
                    kind, owner = expected_kind, expected_owner
                elif (kind, owner) != (expected_kind, expected_owner):
                    raise ValueError("runtime closure would rewrite existing physical ownership")
            ranges.append({"start": first, "end": end, "kind": kind, "owner": owner})
    catalogue["ranges"] = ranges


def _continuation_cfg(analyzer, start: int, code: list[list[int]], entry: int) -> dict:
    sites = {site for first, end in code for site in range(first, end, 4)}
    queue, visited, returns = [entry], set(), set()
    while queue:
        site = queue.pop()
        if site in visited:
            continue
        if site not in sites:
            raise ValueError("runtime continuation crosses data or its complete section bound")
        instruction = analyzer.decode(site, "arm")
        detail = analyzer.detailed(site)
        if instruction is None or detail is None or instruction.size != 4 or instruction.call or instruction.indirect:
            raise ValueError("runtime continuation has an unproved call/computed transfer")
        visited.add(site)
        if instruction.returns:
            if detail.id != arm.ARM_INS_BX or len(detail.operands) != 1 or detail.operands[0].reg != arm.ARM_REG_LR:
                raise ValueError("runtime continuation does not return through the original restored LR")
            returns.add(site)
            if instruction.conditional:
                queue.append(site + 4)
        elif instruction.branch:
            if instruction.target is None or instruction.target[1] != "arm":
                raise ValueError("runtime continuation branch has no ARM destination")
            queue.append(instruction.target[0])
            if instruction.conditional:
                queue.append(site + 4)
        else:
            if detail.id not in (arm.ARM_INS_MOV, arm.ARM_INS_MVN) or detail.writeback:
                raise ValueError("runtime continuation has an unsupported nonreturn effect")
            writes = detail.regs_access()[1]
            if set(writes) & {arm.ARM_REG_SP, arm.ARM_REG_LR, arm.ARM_REG_R4, arm.ARM_REG_PC}:
                raise ValueError("runtime continuation destroys the restored caller contract")
            queue.append(site + 4)
    if not returns:
        raise ValueError("runtime continuation lacks a complete normal return")
    return {"entry": entry, "instruction_sites": sorted(visited), "return_sites": sorted(returns),
            "returns_via_restored_original_lr": True, "complete_bounded_cfg": True}


def _caller_prefix(analyzer, start: int, code: list[list[int]], descriptor: dict) -> dict:
    """Close every entry-to-exception path before borrowing the saved-pair contract."""
    save_site, call_site = descriptor["save_site"], descriptor["call_site"]
    sites = {site for first, end in code for site in range(first, min(end, call_site + 4), 4)}
    queue = [(start, False, frozenset())]
    visited, returns, called = set(), set(), False
    scalar = {arm.ARM_INS_ASR, arm.ARM_INS_LSL, arm.ARM_INS_LSR, arm.ARM_INS_ORR,
              arm.ARM_INS_RSB, arm.ARM_INS_SUB, arm.ARM_INS_CMP, arm.ARM_INS_MOV}
    processed = 0
    while queue:
        processed += 1
        if processed > 4096:
            raise ValueError("runtime caller prefix exceeds its complete-path bound")
        site, saved, history = queue.pop()
        if site in history or site not in sites:
            raise ValueError("runtime caller prefix loops, crosses data or escapes its section")
        instruction, detail = analyzer.decode(site, "arm"), analyzer.detailed(site)
        if instruction is None or detail is None or instruction.size != 4:
            raise ValueError("runtime caller prefix is not complete native ARM code")
        visited.add(site)
        history = history | {site}
        if site == save_site:
            if (saved or detail.id != arm.ARM_INS_PUSH or detail.cc != arm.ARM_CC_AL
                    or [op.reg for op in detail.operands] != [arm.ARM_REG_R4, arm.ARM_REG_LR]):
                raise ValueError("runtime caller does not save its original r4/LR pair exactly once")
            queue.append((site + 4, True, history))
        elif site == call_site:
            if (not saved or not instruction.call or instruction.conditional
                    or instruction.target != (descriptor["dispatcher"], "arm")):
                raise ValueError("runtime exceptional dispatcher does not receive the original saved LR")
            called = True
        elif instruction.returns:
            if saved or detail.id != arm.ARM_INS_BX or detail.operands[0].reg != arm.ARM_REG_LR:
                raise ValueError("runtime caller prefix loses its original return contract")
            returns.add(site)
            if instruction.conditional:
                queue.append((site + 4, saved, history))
        elif instruction.branch:
            if instruction.target is None or instruction.target[1] != "arm":
                raise ValueError("runtime caller prefix has an opaque branch")
            queue.append((instruction.target[0], saved, history))
            if instruction.conditional:
                queue.append((site + 4, saved, history))
        else:
            if (instruction.call or instruction.indirect or detail.id not in scalar or detail.writeback
                    or set(detail.regs_access()[1]) & {arm.ARM_REG_R4, arm.ARM_REG_LR, arm.ARM_REG_SP, arm.ARM_REG_PC}):
                raise ValueError("runtime caller prefix has an unproved register or memory effect")
            queue.append((site + 4, saved, history))
    if not called or visited != sites:
        raise ValueError("runtime caller prefix fails complete code/exceptional-path coverage")
    return {"entry": start, "instruction_sites": sorted(visited), "normal_return_sites": sorted(returns),
            "exceptional_call": call_site, "saved_pair_dominates_every_exceptional_call": True,
            "saved_pair_is_original_r4_lr": True}


def close_catalogue(catalogue: dict, analyzer, evidence: dict, protected: dict[int, dict]) -> dict:
    """Close only frozen complete section/data objects and exact descriptor paths.

    Data placement uses owned code references, never an image content search or an
    index guessed from appearance. A section witness alone cannot make an API alias,
    grow a protected function or establish a globally nonreturning dispatcher.
    """
    if catalogue["image_sha256"] != evidence["image_sha256"]:
        raise ValueError("runtime closure belongs to another original image")
    functions = {function["address"]: function for function in catalogue["functions"]}
    replacements, applied, extent_repairs, removed_anomalies = [], [], [], []
    for record in evidence["sections"]:
        _check_binding(catalogue, analyzer, record)
        _check_section(analyzer, record)
        base = record["binding"]["address"]
        section = record["section"]
        partitions = section["partitions"]
        data = [[base + part["offset"], base + part["end_offset"]] for part in partitions if part["kind"] == "data"]
        if record["closure_kind"] == "unowned_data":
            _check_data_reference(catalogue, analyzer, record)
            first, end = record["claim"]
            if (not any(lo <= first < end <= hi for lo, hi in data)
                    or any(any(lo < end and first < hi for lo, hi in function["code_ranges"])
                           or any(first <= entry["address"] < end for entry in function["entries"])
                           for function in functions.values())):
                raise ValueError("runtime data closure crosses an owned instruction or entry")
            replacements.append((first, end, "rodata", None))
            applied.append({"id": record["id"], "start": first, "end": end, "kind": "rodata", "owner": None,
                            "section_base": base, "section_size": section["size"], "whole_object_bounds_equal": True,
                            "reference": record["data_reference"], "object_witness": record["object_witness"],
                            "qualification": record["qualification"]})
            continue
        function = functions[base]
        if base in protected or any(owner["address"] != base and base < owner["address"] + owner["size"]
                                    and owner["address"] < base + section["size"] for owner in functions.values()):
            raise ValueError("runtime continuation would change a protected or foreign function extent")
        expected = record["prior_function"]
        if function["entries"] != expected["entries"] or function["isa"] != "arm":
            raise ValueError("runtime continuation cannot create or erase an existing entry")
        old_size = expected["size"]
        if function["size"] not in (old_size, section["size"]):
            raise ValueError("runtime continuation's prior extent changed without a witness")
        if not all(any(part["offset"] <= lo - base < hi - base <= part["end_offset"]
                       for part in partitions if part["kind"] == "code") for lo, hi in function["code_ranges"]):
            raise ValueError("runtime continuation section mapping would recast old code as data")
        descriptor = record["descriptor"]
        call = analyzer.decode(descriptor["call_site"], "arm")
        save = analyzer.detailed(descriptor["save_site"])
        dispatcher = functions.get(descriptor["dispatcher"])
        if (call is None or call.conditional or not call.call or call.target != (descriptor["dispatcher"], "arm")
                or descriptor["address"] != descriptor["call_site"] + 4
                or save is None or save.id != arm.ARM_INS_PUSH or save.cc != arm.ARM_CC_AL
                or [operand.reg for operand in save.operands] != [arm.ARM_REG_R4, arm.ARM_REG_LR]
                or descriptor["save_site"] + 4 != descriptor["call_site"]
                or dispatcher is None or dispatcher["size"] != descriptor["dispatcher_size"]
                or dispatcher.get("classification_evidence", {}).get("identity") != descriptor["dispatcher_identity"]
                or _sha(_read(analyzer, dispatcher["address"], dispatcher["address"] + dispatcher["size"]))
                != descriptor["dispatcher_image_sha256"]):
            raise ValueError("runtime exceptional call lacks exact caller/dispatcher identity and saved LR")
        proof = descriptor_paths(analyzer, descriptor["dispatcher"], descriptor["dispatcher_size"], descriptor["address"])
        if proof["continuation_targets"] != descriptor["continuation_targets"] or data != [descriptor["data_range"]]:
            raise ValueError("runtime descriptor width or every-path continuation differs")
        code = [[base + part["offset"], base + part["end_offset"]] for part in partitions if part["kind"] == "code"]
        prefix = _caller_prefix(analyzer, base, code, descriptor)
        continuation = [_continuation_cfg(analyzer, base, code, target) for target in proof["continuation_targets"]]
        added_sites = {site for first, end in code for site in range(max(first, base + old_size), end, 4)}
        if added_sites != {site for row in continuation for site in row["instruction_sites"] if site >= base + old_size}:
            raise ValueError("runtime continuation does not account for every added instruction")
        if function["size"] == old_size:
            if function.get("classification_evidence", expected["classification_evidence"]) != expected["classification_evidence"]:
                raise ValueError("runtime historical classification evidence was changed")
            function.update(size=section["size"], code_size=max(hi for _, hi in code) - base,
                            code_ranges=code, pools=[], literals=data)
            function["runtime_extent_evidence"] = {"id": record["id"], "old_size": old_size,
                                                  "new_size": section["size"], "historical_classification": expected["classification_evidence"],
                                                  "whole_section_sha256": section["image_sha256"]}
            catalogue["stats"]["code_bytes"] += len(added_sites) * 4
            catalogue["stats"]["literal_bytes"] += sum(hi - lo for lo, hi in data)
            catalogue["stats"]["decoded_instructions"] += len(added_sites)
            catalogue["stats"]["functions_with_literals"] += 1
            kinds = catalogue["stats"]["literal_objects_by_kind"]
            kinds["runtime_descriptor"] = kinds.get("runtime_descriptor", 0) + 1
        for first, end in code + data:
            replacements.append((first, end, "function", base))
        removed = [issue for issue in catalogue["anomalies"]
                   if issue == {"address": descriptor["address"], "issue": "undecodable_reachable_code"}]
        catalogue["anomalies"] = [issue for issue in catalogue["anomalies"] if issue not in removed]
        removed_anomalies += removed
        extent_repairs.append({"address": base, "old_size": old_size, "size": section["size"], "isa": "arm",
                               "basis": "exact_runtime_section_and_descriptor_continuation", "entries_unchanged": True,
                               "ordinary_C_credit_delta": 0})
        applied.append({"id": record["id"], "start": base + old_size, "end": base + section["size"],
                        "kind": "function", "owner": base, "code_ranges_added": [[base + old_size + 4, base + section["size"]]],
                        "descriptor_range": data[0], "descriptor_proof": proof, "caller_prefix": prefix,
                        "continuation_cfgs": continuation,
                        "historical_classification": expected["classification_evidence"], "object_witness": record["object_witness"],
                        "qualification": record["qualification"]})
    _replace_unknown_ranges(catalogue, replacements)
    stats = catalogue["stats"]
    stats.update(bytes_by_range_kind=dict(sorted(Counter({kind: sum(row["end"] - row["start"] for row in catalogue["ranges"]
                                                                    if row["kind"] == kind)
                                                          for kind in {row["kind"] for row in catalogue["ranges"]}}).items())),
                 unknown_ranges=sum(row["kind"] == "unknown" for row in catalogue["ranges"]),
                 anomalies=len(catalogue["anomalies"]),
                 anomalies_by_issue=dict(sorted(Counter(row["issue"] for row in catalogue["anomalies"]).items())))
    result = {"sections": applied, "extent_repairs": extent_repairs, "resolved_anomalies": removed_anomalies,
              "new_C_credit": 0, "global_noreturn_inferred": False}
    catalogue["closure_evidence"]["exact_runtime_data"] = result
    return result


def classification_records(records: list[dict], evidence: dict) -> list[dict]:
    """Supersede only the separately evidenced new extent; retain the old proof."""
    replacements = {record["binding"]["address"]: record for record in evidence["sections"]
                    if record["closure_kind"] == "descriptor_continuation"}
    result = []
    for classification in records:
        record = replacements.get(classification["address"])
        if record is None:
            result.append(classification)
            continue
        historical = record["prior_function"]["classification_evidence"]
        if classification == record["classification_record"]:
            result.append(classification)
        elif classification == historical:
            result.append(record["classification_record"])
        else:
            raise ValueError("runtime extent repair cannot rewrite a different historical identity")
    if replacements.keys() - {row["address"] for row in result}:
        raise ValueError("runtime extent repair lost its prior classification record")
    result.extend(record["classification_record"] for record in evidence.get("entry_fragments", []))
    return result


def close_entry_fragments(catalogue: dict, analyzer, evidence: dict, protected: dict[int, dict]) -> list[dict]:
    """Admit real global declarations only at independently bound section offsets."""
    functions = {function["address"]: function for function in catalogue["functions"]}
    replacements, applied = [], []
    for record in evidence.get("entry_fragments", []):
        address, size = record["address"], record["size"]
        section, anchor = record["section"], record["anchor"]
        bound = functions.get(anchor["address"])
        base = anchor["address"] - anchor["symbol_offset"]
        if (record["isa"] != "thumb" or size != 2 or address != base + record["symbol_offset"]
                or section["section_base"] != base or base % section["alignment"]
                or not section["whole_section_reencoded_equal"] or bound is None
                or (bound["size"], bound["isa"]) != (anchor["size"], anchor["isa"])
                or _record_sha(bound) != anchor["record_sha256"]
                or bound.get("classification_evidence", {}).get("identity") != anchor["symbol"]
                or _sha(_read(analyzer, base, base + section["section_size"])) != section["image_section_sha256"]
                or _sha(_read(analyzer, anchor["address"], anchor["address"] + anchor["size"])) != anchor["image_sha256"]):
            raise ValueError("selector entry lacks complete independently bound section/anchor equality")
        declaration = [symbol for symbol in section["symbols"] if symbol["name"] == record["symbol"]]
        if (len(declaration) != 1 or declaration[0]["kind"] != "STT_FUNC" or declaration[0]["bind"] != "STB_GLOBAL"
                or declaration[0]["offset"] != record["symbol_offset"] or declaration[0]["size"] != size):
            raise ValueError("selector entry has no actual exact-sized global function declaration")
        classification = record["classification_record"]
        if (classification["address"] != address or classification["size"] != size
                or classification["isa"] != record["isa"] or classification["kind"] != "library"
                or classification["basis"] != "known_library" or classification["identity"] != record["symbol"]
                or classification["image_sha256"] != catalogue["image_sha256"]
                or classification["original_extent_sha256"] != record["fragment_sha256"]):
            raise ValueError("selector classification is not its independent complete declaration")
        offset = address - base
        mapping = section["mapping"]
        selected = next(((first, mapping[index + 1][0] if index + 1 < len(mapping) else section["section_size"], kind)
                         for index, (first, kind) in enumerate(mapping)
                         if first <= offset < (mapping[index + 1][0] if index + 1 < len(mapping) else section["section_size"])), None)
        if selected is None or selected[2] != "$t" or offset + size > selected[1]:
            raise ValueError("selector entry is not fully covered by actual Thumb section mapping")
        for relocation in section["relocations"]:
            if relocation["type"] != 10:
                raise ValueError("selector full-section proof contains an unsupported relocation")
            instruction = analyzer.decode(base + relocation["offset"], "thumb")
            if instruction is None or not instruction.call or instruction.target != (
                    relocation["target"], "thumb" if relocation["thumb"] else "arm"):
                raise ValueError("selector full-section original call relocation differs")
        expected_references = [row for row in section["relocations"] if row["agreement"] == "same_section_definition"]
        if not expected_references or record["placement_references"] != expected_references:
            raise ValueError("selector placement omits its independent same-section references")
        for reference in expected_references:
            site = base + reference["offset"]
            if (reference["symbol_section"] != section["section_index"]
                    or reference["target"] - reference["symbol_offset"] != base
                    or not any(first <= site < end for first, end in bound["code_ranges"])):
                raise ValueError("selector same-section reference is not owned authenticated caller code")
        if (address in functions or any(function["address"] < address + size
                                      and address < function["address"] + function["size"] for function in functions.values())
                or any(function["address"] < address + size and address < function["address"] + function["size"]
                       for function in protected.values())):
            raise ValueError("selector entry would replace a protected or existing identity")
        if sum(min(address + size, row["end"]) - max(address, row["start"])
               for row in catalogue["ranges"] if row["kind"] == "unknown"
               and row["start"] < address + size and address < row["end"]) != size:
            raise ValueError("selector entry does not cover an exact independently unknown interval")
        instruction = analyzer.decode(address, "thumb")
        detail = next(analyzer.thumb.disasm(_read(analyzer, address, address + size), address, count=1), None)
        if (instruction is None or detail is None or instruction.size != size or instruction.conditional
                or not instruction.returns or detail.id != arm.ARM_INS_BX
                or detail.operands[0].reg != arm.ARM_REG_LR
                or _sha(_read(analyzer, address, address + size)) != record["fragment_sha256"]):
            raise ValueError("selector declaration is not a complete exact normal-return body")
        function = {"address": address, "size": size, "code_size": size, "isa": "thumb", "kind": "library",
                    "pools": [], "literals": [], "code_ranges": [[address, address + size]],
                    "evidence": ["exact_bound_global_section_entry"],
                    "entries": [{"address": address, "isa": "thumb", "kind": "entry",
                                 "evidence": ["exact_bound_global_section_entry"], "sources": []}],
                    "noreturn": False, "noreturn_reasons": [],
                    "classification_evidence": record["classification_record"]}
        catalogue["functions"].append(function)
        functions[address] = function
        replacements.append((address, address + size, "function", address))
        applied.append({"id": record["id"], "address": address, "size": size, "isa": "thumb",
                        "symbol": record["symbol"], "section_base": base,
                        "section_size": section["section_size"], "whole_section_sha256": section["image_section_sha256"],
                        "anchor_unchanged": anchor, "same_section_references": expected_references,
                        "new_C_credit": 0, "qualification": record["qualification"]})
    if not applied:
        catalogue["closure_evidence"]["exact_runtime_data"]["entry_fragments"] = []
        return applied
    catalogue["functions"].sort(key=lambda function: function["address"])
    _replace_unknown_ranges(catalogue, replacements)
    stats = catalogue["stats"]
    added_bytes = sum(row["size"] for row in applied)
    stats["functions"] = len(catalogue["functions"])
    stats["functions_by_isa"] = dict(sorted(Counter(function["isa"] for function in catalogue["functions"]).items()))
    stats["starts_by_evidence"] = dict(sorted(Counter(item for function in catalogue["functions"]
                                                     for item in function["evidence"]).items()))
    stats["code_bytes"] += added_bytes
    stats["decoded_instructions"] += len(applied)
    stats["bytes_by_range_kind"]["unknown"] -= added_bytes
    stats["bytes_by_range_kind"]["function"] += added_bytes
    stats["unknown_ranges"] = sum(row["kind"] == "unknown" for row in catalogue["ranges"])
    catalogue["closure_evidence"]["exact_runtime_data"]["entry_fragments"] = applied
    return applied


def check_prepared_protection(catalogue: dict, evidence: dict) -> None:
    """Bind every currently prepared identity, including inherited literal metadata."""
    snapshot = evidence.get("_prepared_protection")
    if snapshot is None:
        return
    functions = {function["address"]: function for function in catalogue["functions"]}
    if len(snapshot["identities"]) != snapshot["identity_count"]:
        raise ValueError("prepared runtime protection identity count differs")
    for row in snapshot["identities"]:
        function = functions.get(row["owner"])
        if (function is None or (function["size"], function["isa"], function["entries"]) != (
                row["size"], row["isa"], row["entries"])
                or _record_sha(function) != row["proposal_record_sha256"]):
            raise ValueError(f"v11 closure changes prepared identity {row['name']}")
