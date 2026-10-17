// SPDX-License-Identifier: GPL-3.0-or-later
// Catalogue-seeded, local-only pseudo-C author aid. Called by tools/decompile.py.
// @category Kanken
import java.io.InputStream;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.SourceType;

public class DecompileFunction extends GhidraScript {
    private final Gson gson = new Gson();

    private void seed(JsonObject manifest) throws Exception {
        Memory memory = currentProgram.getMemory();
        for (MemoryBlock block : memory.getBlocks()) {
            memory.removeBlock(block, monitor);
        }
        Path binary = Path.of(manifest.get("binary").getAsString());
        for (JsonElement item : manifest.getAsJsonArray("segments")) {
            JsonObject segment = item.getAsJsonObject();
            String name = segment.get("name").getAsString();
            try (InputStream input = Files.newInputStream(binary)) {
                input.skipNBytes(segment.get("offset").getAsLong());
                MemoryBlock block = memory.createInitializedBlock(name,
                    toAddr(segment.get("address").getAsLong()), input,
                    segment.get("size").getAsLong(), monitor, false);
                block.setRead(true);
                block.setWrite(name.equals("data"));
                block.setExecute(name.equals("text"));
            }
        }
        MemoryBlock bss = memory.createUninitializedBlock("bss",
            toAddr(manifest.get("bss_address").getAsLong()),
            manifest.get("bss_size").getAsLong(), false);
        bss.setRead(true);
        bss.setWrite(true);
        bss.setExecute(false);

        Register tmode = currentProgram.getRegister("TMode");
        for (JsonElement item : manifest.getAsJsonArray("functions")) {
            monitor.checkCancelled();
            JsonObject fn = item.getAsJsonObject();
            Address entry = toAddr(fn.get("address").getAsLong());
            AddressSet body = new AddressSet();
            for (JsonElement range : fn.getAsJsonArray("code_ranges")) {
                JsonArray pair = range.getAsJsonArray();
                body.addRange(toAddr(pair.get(0).getAsLong()), toAddr(pair.get(1).getAsLong() - 1));
            }
            if (body.isEmpty() || !body.contains(entry)) {
                continue;
            }
            boolean thumb = fn.get("isa").getAsString().equals("thumb");
            for (AddressRange range : body.getAddressRanges()) {
                currentProgram.getProgramContext().setValue(tmode,
                    range.getMinAddress(), range.getMaxAddress(), thumb ? BigInteger.ONE : BigInteger.ZERO);
            }
            currentProgram.getFunctionManager().createFunction(
                fn.get("name").getAsString(), entry, body, SourceType.USER_DEFINED);
            for (AddressRange range : body.getAddressRanges()) {
                // Decode each code island, never sweep through catalogue literal pools.
                DisassembleCommand command = new DisassembleCommand(range.getMinAddress(), body, true);
                command.enableCodeAnalysis(false);
                command.applyTo(currentProgram, monitor);
            }
        }
        for (JsonElement item : manifest.getAsJsonArray("symbols")) {
            JsonObject symbol = item.getAsJsonObject();
            Address address = toAddr(symbol.get("address").getAsLong());
            if (currentProgram.getFunctionManager().getFunctionAt(address) == null) {
                createLabel(address, symbol.get("name").getAsString(), true);
            }
        }
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 6) {
            throw new IllegalArgumentException("Expected manifest, seed flag, address, seconds, C path, JSON path");
        }
        JsonObject manifest = gson.fromJson(Files.readString(Path.of(args[0])), JsonObject.class);
        if (args[1].equals("seed")) {
            seed(manifest);
        }
        Function function = currentProgram.getFunctionManager().getFunctionAt(toAddr(Long.parseLong(args[2])));
        if (function == null) {
            throw new IllegalStateException("No catalogue-seeded function at requested address");
        }
        DecompInterface decompiler = new DecompInterface();
        JsonObject metadata = new JsonObject();
        try {
            decompiler.toggleCCode(true);
            DecompileOptions options = new DecompileOptions();
            options.setRespectReadOnly(true);
            decompiler.setOptions(options);
            decompiler.toggleSyntaxTree(true);
            if (!decompiler.openProgram(currentProgram)) {
                throw new IllegalStateException("Cannot open decompiler: " + decompiler.getLastMessage());
            }
            long start = System.nanoTime();
            DecompileResults result = decompiler.decompileFunction(function, Integer.parseInt(args[3]), monitor);
            metadata.addProperty("decompile_seconds", (System.nanoTime() - start) / 1e9);
            metadata.addProperty("completed", result.decompileCompleted());
            metadata.addProperty("timed_out", result.isTimedOut());
            metadata.addProperty("warning", result.getErrorMessage());
            if (!result.decompileCompleted() || result.getDecompiledFunction() == null) {
                throw new IllegalStateException("Decompiler did not complete; inspect private headless.log");
            }
            String text = "/* PRIVATE author aid: inferred pseudo-C, NOT matching source or an ABI contract. */\n"
                + result.getDecompiledFunction().getC();
            Files.writeString(Path.of(args[4]), text, StandardCharsets.UTF_8);
            metadata.addProperty("function", function.getName());
            metadata.addProperty("address", function.getEntryPoint().getOffset());
            metadata.addProperty("body_bytes", function.getBody().getNumAddresses());
            metadata.addProperty("basic_blocks", result.getHighFunction().getBasicBlocks().size());
            metadata.addProperty("function_count", currentProgram.getFunctionManager().getFunctionCount());
            Files.writeString(Path.of(args[5]), gson.toJson(metadata), StandardCharsets.UTF_8);
        }
        finally {
            decompiler.dispose();
        }
    }
}
