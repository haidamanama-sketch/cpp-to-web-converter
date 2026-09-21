# CPP2WEB

A cross-platform C++ tool that generates a simple HTML + JavaScript + CSS application from C++-style UI definitions and works on Windows, macOS, and Linux.

## One-command Linux installation

The following command downloads the source, builds a Release version, and installs `cpp2web_app` into `~/.local/bin`:

```bash
curl -fsSL https://raw.githubusercontent.com/haidamanama-sketch/cpp-to-web-converter/main/install.sh | bash
```

If `curl` is unavailable, use `wget`:

```bash
wget -qO- https://raw.githubusercontent.com/haidamanama-sketch/cpp-to-web-converter/main/install.sh | bash
```

The installer requires `cmake`, `tar`, and a working C++ compiler. It does not require `sudo` by default. If `~/.local/bin` is not already in `PATH`, run the export command printed by the installer, then use:

```bash
cpp2web_app --output ./demo-output --open
```

To install somewhere else:

```bash
curl -fsSL https://raw.githubusercontent.com/haidamanama-sketch/cpp-to-web-converter/main/install.sh | bash -s -- --prefix "$HOME/.local"
```

For a pinned branch or tag, set `CPP2WEB_REF`:

```bash
CPP2WEB_REF=main curl -fsSL https://raw.githubusercontent.com/haidamanama-sketch/cpp-to-web-converter/main/install.sh | bash
```

## Features

- C++ code generation for browser UI
- Pure C++ header fallback when no graphics library is available
- Cross-platform build via CMake
- Output project with `index.html`, `styles.css`, and `app.js`
- Works as a command-line tool and as a reusable library

## Project structure

- `include/cpp2web/platform.hpp` — OS detection and launch helpers
- `include/cpp2web/web_ui.hpp` — no-graphics fallback UI header
- `include/cpp2web/converter.hpp` — project conversion logic
- `src/converter.cpp` — conversion implementation
- `src/main.cpp` — command-line entry point
- `examples/basic_app.cpp` — sample app
- `install.sh` — Linux one-command installer

## Build from source

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

Run:

```bash
./cpp2web_app --output ../demo-output
```

If you want to open the generated page immediately:

```bash
./cpp2web_app --output ../demo-output --open
```

## Example C++ input

```cpp
#include "cpp2web/web_ui.hpp"

int main() {
    cpp2web::WebApp app("Calculator");
    app.window("Main")
       .button("Add")
       .button("Subtract")
       .text("Result: 0");

    app.exportProject("./output");
    return 0;
}
```

Generated output includes:
- `index.html`
- `styles.css`
- `app.js`

## Notes

- This project focuses on a pragmatic C++ UI abstraction and web export.
- If your environment does not have a GUI library like Qt, SDL, or OpenGL, the included header provides a lightweight fallback.
- The project is intentionally cross-platform and uses only standard C++17 features.
