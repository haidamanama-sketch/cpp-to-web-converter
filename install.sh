#!/usr/bin/env bash
set -Eeuo pipefail

REPO="haidamanama-sketch/cpp-to-web-converter"
REF="${CPP2WEB_REF:-main}"
PREFIX="${CPP2WEB_PREFIX:-$HOME/.local}"

usage() {
    cat <<EOF
Usage: install.sh [--prefix DIR] [--ref REF]

Installs cpp2web_app into PREFIX/bin.
Defaults:
  PREFIX=$HOME/.local
  REF=$REF

Environment variables CPP2WEB_PREFIX and CPP2WEB_REF can also be used.
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --prefix)
            [[ $# -ge 2 ]] || { echo "Missing value for --prefix" >&2; exit 2; }
            PREFIX="$2"
            shift 2
            ;;
        --ref)
            [[ $# -ge 2 ]] || { echo "Missing value for --ref" >&2; exit 2; }
            REF="$2"
            shift 2
            ;;
        --help|-h)
            usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
done

for command in tar cmake; do
    if ! command -v "$command" >/dev/null 2>&1; then
        echo "Error: '$command' is required. Install it with your Linux distribution's package manager." >&2
        exit 1
    fi
done

if command -v curl >/dev/null 2>&1; then
    fetch() { curl --fail --silent --show-error --location "$1" --output "$2"; }
elif command -v wget >/dev/null 2>&1; then
    fetch() { wget --quiet --output-document="$2" "$1"; }
else
    echo "Error: curl or wget is required." >&2
    exit 1
fi

TMP_DIR="$(mktemp -d "${TMPDIR:-/tmp}/cpp2web-install.XXXXXX")"
cleanup() { rm -rf "$TMP_DIR"; }
trap cleanup EXIT

ARCHIVE="$TMP_DIR/cpp2web.tar.gz"
URL="https://github.com/${REPO}/archive/refs/heads/${REF}.tar.gz"

echo "Downloading CPP2WEB (${REF})..."
fetch "$URL" "$ARCHIVE"
tar -xzf "$ARCHIVE" -C "$TMP_DIR"
SOURCE_DIR="$(find "$TMP_DIR" -mindepth 1 -maxdepth 1 -type d | head -n 1)"

[[ -n "$SOURCE_DIR" && -d "$SOURCE_DIR" ]] || {
    echo "Error: downloaded archive did not contain a source directory." >&2
    exit 1
}

BUILD_DIR="$TMP_DIR/build"
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$PREFIX"
cmake --build "$BUILD_DIR" --config Release --parallel
cmake --install "$BUILD_DIR"

BIN="$PREFIX/bin/cpp2web_app"
if [[ ! -x "$BIN" ]]; then
    echo "Error: installation completed but '$BIN' was not found." >&2
    exit 1
fi

echo
echo "CPP2WEB installed successfully: $BIN"
if [[ ":$PATH:" != *":$PREFIX/bin:"* ]]; then
    echo "Add it to your current shell with:"
    echo "  export PATH=\"$PREFIX/bin:\$PATH\""
    echo
    echo "Then open a new shell or run the export command above."
fi

echo
echo "Try it with: cpp2web_app --output ./demo-output --open"
