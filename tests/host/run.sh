#!/bin/sh
# Host tests of the Amiga port's change to Pomme (no SDL needed): Pomme's
# file layer built with -D__amigaos__, its preferences folder checked.
# MIT, Copyright (c) 2026 AmiCradle Team.
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$HERE/../.." && pwd)
P="$ROOT/extern/Pomme/src"
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT
CXX=${CXX:-g++}
DEFS="-D__amigaos__ -DPOMME_NO_VIDEO -DPOMME_NO_INPUT -DPOMME_NO_GRAPHICS -DPOMME_NO_QD3D \
      -DPOMME_NO_MP3 -DPOMME_CASE_SENSITIVE_FSSPEC=1 -DPOMME_NO_SOUND_MIXER -DPOMME_NO_SOUND_FORMATS"
$CXX -std=c++20 -Wno-multichar $DEFS -I"$P" -o "$WORK/test_prefs" "$HERE/test_prefs.cpp" \
    "$P/Pomme.cpp" "$P/PommeDebug.cpp" "$P/Files/Files.cpp" "$P/Files/HostVolume.cpp" \
    "$P/Files/Resources.cpp" "$P/Memory/Memory.cpp" "$P/Text/TextUtilities.cpp" \
    "$P/Time/TimeManager.cpp" "$P/Utilities/bigendianstreams.cpp" \
    "$P/Utilities/IEEEExtended.cpp" "$P/Utilities/memstream.cpp" \
    "$P/Utilities/StringUtils.cpp" "$P/Utilities/structpack.cpp"
mkdir -p "$WORK/drawer"
"$WORK/test_prefs" "$WORK/drawer"
echo "ALL PASS"
