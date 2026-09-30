#!/bin/bash
# Builds PDFViewer/PDFLib/RDDLib.xcframework from the vendor PDFViewer/PDFLib/libRDDLib.a.
#
# The vendor library's arm64 slice is built for devices only, so the simulator
# slice is made by retagging a copy of those objects as iOS-simulator
# (see arm64_to_sim.py). Re-run this whenever libRDDLib.a is updated.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC="$ROOT/PDFViewer/PDFLib/libRDDLib.a"
OUT="$ROOT/PDFViewer/PDFLib/RDDLib.xcframework"
MINOS="15.0"

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT
mkdir -p "$WORK/device" "$WORK/sim" "$WORK/objs"

lipo "$SRC" -thin arm64 -output "$WORK/device/libRDDLib.a"
lipo "$SRC" -thin x86_64 -output "$WORK/x86_64.a"

# Extract the device objects, retag them, and re-archive in the original order.
(cd "$WORK/objs" && ar -x "$WORK/device/libRDDLib.a")
members=()
while IFS= read -r m; do
  [[ "$m" == __.SYMDEF* ]] || members+=("$m")
done < <(ar -t "$WORK/device/libRDDLib.a")
(cd "$WORK/objs" && python3 "$ROOT/scripts/arm64_to_sim.py" "$MINOS" "${members[@]}")
(cd "$WORK/objs" && xcrun libtool -static -no_warning_for_no_symbols -o "$WORK/arm64-sim.a" "${members[@]}")

lipo -create "$WORK/arm64-sim.a" "$WORK/x86_64.a" -output "$WORK/sim/libRDDLib.a"

rm -rf "$OUT"
xcodebuild -create-xcframework \
  -library "$WORK/device/libRDDLib.a" \
  -library "$WORK/sim/libRDDLib.a" \
  -output "$OUT"
