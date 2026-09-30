#!/usr/bin/env python3
"""Retag arm64 iOS-device Mach-O object files as iOS-simulator objects.

libRDDLib.a ships an arm64 slice built for devices only, so it can't be linked
into an arm64 simulator build. This replaces each object's
LC_VERSION_MIN_IPHONEOS (16 bytes) with an LC_BUILD_VERSION (24 bytes) for
PLATFORM_IOSSIMULATOR. The extra 8 bytes are inserted right after the load
commands, and every file offset that points past them is shifted by 8.

Usage: arm64_to_sim.py <minos> <file.o>...   (files are rewritten in place)
"""
import struct
import sys

MH_MAGIC_64 = 0xFEEDFACF
MH_OBJECT = 0x1
CPU_TYPE_ARM64 = 0x0100000C

LC_SEGMENT_64 = 0x19
LC_SYMTAB = 0x2
LC_DYSYMTAB = 0xB
LC_VERSION_MIN_IPHONEOS = 0x25
LC_BUILD_VERSION = 0x32
LINKEDIT_DATA_CMDS = {
    0x1E,  # LC_SEGMENT_SPLIT_INFO
    0x26,  # LC_FUNCTION_STARTS
    0x29,  # LC_DATA_IN_CODE
    0x2B,  # LC_DYLIB_CODE_SIGN_DRS
    0x2E,  # LC_LINKER_OPTIMIZATION_HINT
}
PLATFORM_IOSSIMULATOR = 7
SECTION_TYPE = 0xFF
ZEROFILL_TYPES = {0x1, 0xC, 0x12}  # S_ZEROFILL, S_GB_ZEROFILL, S_THREAD_LOCAL_ZEROFILL


def encode_version(v):
    parts = [int(p) for p in v.split(".")] + [0, 0]
    return (parts[0] << 16) | (parts[1] << 8) | parts[2]


def shift(value, delta):
    return value + delta if value else 0


def convert(data, minos):
    magic, cputype, _, filetype, ncmds, sizeofcmds, _, _ = struct.unpack_from("<8I", data, 0)
    if magic != MH_MAGIC_64 or cputype != CPU_TYPE_ARM64 or filetype != MH_OBJECT:
        raise ValueError("not an arm64 MH_OBJECT")

    header_size = 32
    delta = 24 - 16
    out_cmds = bytearray()
    converted = False
    off = header_size
    for _ in range(ncmds):
        cmd, cmdsize = struct.unpack_from("<2I", data, off)
        raw = bytearray(data[off:off + cmdsize])
        if cmd == LC_VERSION_MIN_IPHONEOS:
            _, _, _, sdk = struct.unpack_from("<4I", raw, 0)
            raw = bytearray(struct.pack("<6I", LC_BUILD_VERSION, 24, PLATFORM_IOSSIMULATOR,
                                        encode_version(minos), sdk, 0))
            converted = True
        elif cmd == LC_BUILD_VERSION:
            raise ValueError("already has LC_BUILD_VERSION")
        elif cmd == LC_SEGMENT_64:
            fileoff = struct.unpack_from("<Q", raw, 40)[0]
            struct.pack_into("<Q", raw, 40, shift(fileoff, delta))
            nsects = struct.unpack_from("<I", raw, 64)[0]
            for i in range(nsects):
                s = 72 + i * 80
                offset, _, reloff, nreloc, flags = struct.unpack_from("<5I", raw, s + 48)
                if (flags & SECTION_TYPE) not in ZEROFILL_TYPES:
                    struct.pack_into("<I", raw, s + 48, shift(offset, delta))
                if nreloc:
                    struct.pack_into("<I", raw, s + 56, reloff + delta)
        elif cmd == LC_SYMTAB:
            symoff, nsyms, stroff, strsize = struct.unpack_from("<4I", raw, 8)
            struct.pack_into("<4I", raw, 8, shift(symoff, delta), nsyms, shift(stroff, delta), strsize)
        elif cmd == LC_DYSYMTAB:
            # tocoff, modtaboff, extrefsymoff, indirectsymoff, extreloff, locreloff
            for field in (32, 40, 48, 56, 64, 72):
                v = struct.unpack_from("<I", raw, field)[0]
                struct.pack_into("<I", raw, field, shift(v, delta))
        elif cmd in LINKEDIT_DATA_CMDS:
            dataoff, datasize = struct.unpack_from("<2I", raw, 8)
            struct.pack_into("<2I", raw, 8, shift(dataoff, delta), datasize)
        out_cmds += raw
        off += cmdsize

    if not converted:
        raise ValueError("no LC_VERSION_MIN_IPHONEOS found")

    header = bytearray(data[:header_size])
    struct.pack_into("<I", header, 20, sizeofcmds + delta)
    return bytes(header) + bytes(out_cmds) + data[header_size + sizeofcmds:]


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    minos = sys.argv[1]
    for path in sys.argv[2:]:
        with open(path, "rb") as f:
            data = f.read()
        try:
            data = convert(data, minos)
        except ValueError as e:
            sys.exit(f"{path}: {e}")
        with open(path, "wb") as f:
            f.write(data)


if __name__ == "__main__":
    main()
