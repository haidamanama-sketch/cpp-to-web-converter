# CPP2WEB

CPP2WEB is a small, dependency-free C++17 tool for producing usable static web interfaces. It is not a pretend C++ compiler: it intentionally supports a clear UI DSL, escapes generated content, uses semantic HTML, and produces files that can be hosted anywhere.



## Supported input

CPP2WEB deliberately supports a small, predictable DSL instead of claiming to translate arbitrary C++:

```cpp
#include "cpp2web/web_ui.hpp"

int main() {
    cpp2web::WebApp app("Contact form");
    app.window("Contact")
       .text("Tell us how to reach you")
       .input("email", "you@example.com")
       .button("Send", "send-form");
    app.exportProject("./web");
}
```

When using the command-line converter, the same calls are read from a `.cpp` file. It generates:

- `index.html` with labels, inputs, buttons, accessibility attributes, and escaped text
- `styles.css` with responsive and dark-mode styling
- `app.js` with safe `textContent` updates and input collection

Supported calls:

- `.window("title")`
- `.text("content")`
- `.input("id", "placeholder")`
- `.button("label", "action")` (the action argument is optional)

## Build from source

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/cpp2web_app --input examples/basic_app.cpp --output ./web
```

On Windows with Visual Studio:

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\cpp2web_app.exe --input examples\basic_app.cpp --output web
```

On macOS:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/cpp2web_app --input examples/basic_app.cpp --output web --open
```

## Scope and roadmap

The converter is intentionally honest about scope. It does not parse arbitrary C++ or execute C++ in the browser. The next useful additions are repeatable components, forms with validation, a JSON data interface, and tests for generated output.
