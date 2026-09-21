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
