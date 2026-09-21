# CPP2WEB

A cross-platform C++ tool that generates a simple HTML + JavaScript + CSS application from C++-style UI definitions and works on Windows, macOS, and Linux.

Features:
- C++ code generation for browser UI
- Pure C++ header fallback when no graphics library is available
- Cross-platform build via CMake
- Output project with `index.html`, `styles.css`, and `app.js`
- Works as a command-line tool and as a reusable library

Project structure:
- `include/cpp2web/platform.hpp` — OS detection and launch helpers
- `include/cpp2web/web_ui.hpp` — no-graphics fallback UI header
- `include/cpp2web/converter.hpp` — project conversion logic
- `src/converter.cpp` — conversion implementation
- `src/main.cpp` — command-line entry point
- `examples/basic_app.cpp` — sample app

Build:

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

Example C++ input:

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

Notes:
- This project focuses on a pragmatic C++ UI abstraction and web export.
- If your environment does not have a GUI library like Qt, SDL, or OpenGL, the included header provides a lightweight fallback.
- The project is intentionally cross-platform and uses only standard C++17 features.
