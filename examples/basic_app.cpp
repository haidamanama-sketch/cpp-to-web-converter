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
