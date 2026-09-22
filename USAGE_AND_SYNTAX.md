# CPP2WEB 使用方法与新增 C++ 语法说明

本文档整理了该项目的基本使用方式，以及当前已支持的 C++ DSL 语法。

## 1. 安装与运行

### 方式 1：一键安装

```bash
curl -fsSL https://raw.githubusercontent.com/haidamanama-sketch/cpp-to-web-converter/main/install.sh | bash
```

或者使用 wget：

```bash
wget -qO- https://raw.githubusercontent.com/haidamanama-sketch/cpp-to-web-converter/main/install.sh | bash
```

安装后，命令行工具会被放到 `~/.local/bin`，可直接运行：

```bash
cpp2web_app --input app.cpp --output web --open
```

### 方式 2：从源码编译

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/cpp2web_app --input examples/basic_app.cpp --output ./web
```

## 2. 最小用法示例

创建一个示例文件 `app.cpp`：

```cpp
#include "cpp2web/web_ui.hpp"

int main() {
    cpp2web::WebApp app("Contact form");
    app.window("Contact")
        .text("Tell us how to reach you")
        .input("email", "you@example.com")
        .button("Send", "send-form");
    app.exportProject("./web");
    return 0;
}
```

执行：

```bash
cpp2web_app --input app.cpp --output web --open
```

生成内容包括：

- `index.html`
- `styles.css`
- `app.js`

打开输出目录后，即可查看静态网页效果。

## 3. 当前支持的 C++ 语法

这个项目不是“任意 C++ 语法转换器”，而是一个小型、明确的 UI DSL。它主要支持以下调用链：

### 3.1 创建应用

```cpp
cpp2web::WebApp app("App title");
```

说明：

- `WebApp` 是入口对象。
- 构造参数是网页标题。

### 3.2 添加窗口标题

```cpp
app.window("Contact");
```

说明：

- 生成一个 `h2` 标题块。
- 可重复调用以增加多个窗口标题。

### 3.3 添加文本内容

```cpp
app.text("Tell us how to reach you");
```

说明：

- 会生成输出区域中的段落文本。
- 自动做 HTML 转义，避免脚本注入。

### 3.4 添加输入框

```cpp
app.input("email", "you@example.com");
```

说明：

- 第一个参数：字段 id
- 第二个参数：占位文本（可省略）
- 会生成：
  - `<label>`
  - `<input>`
  - `name`、`id` 与 `placeholder`

### 3.5 添加按钮

```cpp
app.button("Send");
app.button("Send", "send-form");
```

说明：

- 第一个参数：按钮文本
- 第二个参数：按钮动作名（可选）
- 生成 `button` 元素，并在 JavaScript 中用 `data-action` 处理点击行为。

### 3.6 导出项目

```cpp
app.exportProject("./web");
```

说明：

- 自动创建目录并生成：
  - `index.html`
  - `styles.css`
  - `app.js`

## 4. 支持的链式写法

当前代码的核心能力是链式调用：

```cpp
cpp2web::WebApp app("Contact form");
app.window("Contact")
   .text("Tell us how to reach you")
   .input("email", "you@example.com")
   .button("Send", "send-form");
```

这是推荐写法，因为它最符合当前项目的 DSL 设计。

## 5. 新增/扩展的语法特点

以下规则是当前项目中新增并明确支持的语法约定：

```cpp
WebApp app("Title");
app.window("Section Title");
app.text("Text content");
app.input("field_id", "placeholder");
app.button("Label");
app.button("Label", "action_name");
app.exportProject("./output");
```

### 语法要点

1. 所有 UI 构造都通过 `cpp2web::WebApp` 对象操作。
2. 多数调用支持链式编程风格。
3. 只有少量“网页 UI DSL”语法被支持，而不是完整的 C++ 语法树。
4. 生成的 HTML 会经过转义处理，避免直接嵌入不可信内容造成 XSS 问题。

## 6. 设计边界

这个项目的目标是“把简单 UI 代码翻译成静态网页”，而不是“支持任意 C++ 程序”。因此，当前版本明确不支持：

- 通用 C++ 控制流解析
- 复杂类/模板/继承结构
- 任意函数内联执行语义
- 浏览器端真实的 C++ 运行环境

## 7. 示例文件

当前仓库中已提供示例：

```bash
examples/basic_app.cpp
```

可直接参考：

```cpp
#include "cpp2web/web_ui.hpp"

int main() {
    cpp2web::WebApp app("Contact form");
    app.window("Contact")
        .text("Tell us how to reach you")
        .input("email", "you@example.com")
        .button("Send", "send-form");
    app.exportProject("./output");
    return 0;
}
```

## 8. 常见使用建议

- 一次只写少量 UI 元素，便于维护。
- 按 ID 命名输入框，便于后续扩展。
- 使用链式调用，使 C++ 代码更接近声明式 UI。
- 在生成的静态页面中，按钮逻辑由 JavaScript 处理，不依赖浏览器执行 C++。

如果你需要进一步扩展该项目，比如增加 `checkbox`、`select`、`textarea`、`list` 等更多控件，可以按同样模式在 `WebApp` 中新增方法并在 HTML/CSS/JS 生成器中同步实现。
