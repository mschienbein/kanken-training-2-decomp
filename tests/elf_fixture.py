"""Independent synthetic ELF32 REL writer adapted from the sealed U1 review reproductions."""
from __future__ import annotations

import struct

import inplace


TYPES = {"SHT_PROGBITS": 1, "SHT_NOBITS": 8, "SHT_ARM_EXIDX": 0x70000001}
KINDS = {"STT_NOTYPE": 0, "STT_OBJECT": 1, "STT_FUNC": 2, "STT_SECTION": 3}
BINDS = {"STB_LOCAL": 0, "STB_GLOBAL": 1, "STB_WEAK": 2}
SPECIAL_INDICES = {"SHN_UNDEF": 0, "SHN_ABS": 0xFFF1}


def serialize_object(obj: inplace.ObjectFile) -> bytes:
    """Emit only independently constructed test metadata/payload; no licensed object inputs."""
    symbols = list(obj.symbols)
    for section in obj.sections.values():
        for relocation in section.relocs:
            if not any(symbol is relocation.symbol for symbol in symbols):
                symbols.append(relocation.symbol)
        for offset, marker in section.mapping:
            if not any(symbol.name == marker and symbol.value == offset and symbol.shndx == section.index
                       for symbol in symbols):
                symbols.append(inplace.Symbol(marker, offset, 0, "STT_NOTYPE", "STB_LOCAL", section.index))
    symbols.sort(key=lambda symbol: BINDS[symbol.bind] != 0)
    symbol_indices = {id(symbol): index + 1 for index, symbol in enumerate(symbols)}
    indices = {index: serial + 1 for serial, index in enumerate(obj.sections)}
    sections = [{"name": "", "type": 0, "flags": 0, "data": b"", "size": 0}]
    for section in obj.sections.values():
        flags = 2 | int(section.writable) | (4 if section.executable else 0)
        flags |= 0x80 if section.link_order else 0
        flags |= 0x30 if section.merge_strings else 0
        flags |= 0x200 if section.comdat else 0
        flags |= 0x200000 if section.retention else 0
        sections.append({
            "name": section.name, "type": TYPES[section.elf_type], "flags": flags,
            "data": section.data, "size": section.size, "align": section.align,
            "entsize": section.entsize,
            "link": indices.get(section.linked_section, section.linked_section) if section.link_order else 0,
        })
    for _, signature, members in getattr(obj, "section_groups", []):
        if signature and not any(symbol.name == signature for symbol in symbols):
            symbol = inplace.Symbol(signature, 0, 0, "STT_NOTYPE", "STB_GLOBAL", "SHN_ABS")
            symbols.append(symbol)
            symbol_indices[id(symbol)] = len(symbols)
        for member in sorted(members, key=str):
            if member not in indices:
                indices[member] = len(sections)
                sections.append({"name": ".zero.unrepresented", "type": 1, "flags": 2,
                                 "data": b"", "size": 0, "align": 1})
    strings = bytearray(b"\0")
    symbol_table = bytearray(16)
    for symbol in symbols:
        name = len(strings)
        strings.extend(symbol.name.encode() + b"\0")
        index = indices[symbol.shndx] if symbol.shndx in indices else SPECIAL_INDICES[symbol.shndx]
        symbol_table.extend(struct.pack("<IIIBBH", name, symbol.value, symbol.size,
                                        (BINDS[symbol.bind] << 4) | KINDS[symbol.kind], 0, index))
    symbol_table_index = len(sections)
    sections.extend([
        {"name": ".symtab", "type": 2, "flags": 0, "data": bytes(symbol_table), "size": len(symbol_table),
         "link": symbol_table_index + 1, "info": 1 + sum(symbol.bind == "STB_LOCAL" for symbol in symbols),
         "entsize": 16, "align": 4},
        {"name": ".strtab", "type": 3, "flags": 0, "data": bytes(strings), "size": len(strings)},
    ])
    for index, section in obj.sections.items():
        if section.relocs:
            data = b"".join(struct.pack("<II", relocation.offset,
                                         (symbol_indices[id(relocation.symbol)] << 8) | relocation.type)
                            for relocation in section.relocs)
            sections.append({"name": ".rel" + section.name, "type": 9, "flags": 0, "data": data,
                             "size": len(data), "link": symbol_table_index, "info": indices[index],
                             "entsize": 8, "align": 4})
    for flags, signature, members in getattr(obj, "section_groups", []):
        signature_index = next((index + 1 for index, symbol in enumerate(symbols) if symbol.name == signature), 0)
        words = [flags, *[indices[index] for index in sorted(members, key=str)]]
        data = struct.pack("<" + "I" * len(words), *words)
        sections.append({"name": ".group." + signature, "type": 17, "flags": 0, "data": data,
                         "size": len(data), "link": symbol_table_index, "info": signature_index,
                         "align": 4, "entsize": 4})
    names = bytearray(b"\0")
    for section in sections:
        section["name_offset"] = len(names)
        names.extend(section["name"].encode() + b"\0")
    names_index, names_offset = len(sections), len(names)
    names.extend(b".shstrtab\0")
    sections.append({"name_offset": names_offset, "type": 3, "flags": 0,
                     "data": bytes(names), "size": len(names)})
    blob = bytearray(52)
    for section in sections:
        blob.extend(bytes(-len(blob) % section.get("align", 1)))
        section["offset"] = len(blob)
        if section["type"] != 8:
            blob.extend(section["data"])
    blob.extend(bytes(-len(blob) % 4))
    headers = len(blob)
    for section in sections:
        blob.extend(struct.pack("<10I", section["name_offset"], section["type"], section["flags"], 0,
                                section["offset"], section["size"], section.get("link", 0),
                                section.get("info", 0), section.get("align", 1), section.get("entsize", 0)))
    ident = b"\x7fELF\x01\x01\x01" + bytes(9)
    blob[:52] = struct.pack("<16sHHIIIIIHHHHHH", ident, 1, 40, 1, 0, 0, headers,
                            0x05000000, 52, 0, 0, 40, len(sections), names_index)
    return bytes(blob)
