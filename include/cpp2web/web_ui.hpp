#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace cpp2web {

class WebApp {
public:
    explicit WebApp(std::string title = "C++ to Web Demo")
        : title_(std::move(title)) {}

    WebApp& window(std::string title) {
        windows_.push_back(std::move(title));
        return *this;
    }

    WebApp& button(std::string label) {
        buttons_.push_back(std::move(label));
        return *this;
    }

    WebApp& text(std::string content) {
        labels_.push_back(std::move(content));
        return *this;
    }

    const std::string& title() const { return title_; }
    const std::vector<std::string>& windows() const { return windows_; }
    const std::vector<std::string>& buttons() const { return buttons_; }
    const std::vector<std::string>& labels() const { return labels_; }

    std::string html() const {
        std::ostringstream html;
        html << "<!DOCTYPE html>\n";
        html << "<html lang=\"en\">\n";
        html << "<head>\n";
        html << "  <meta charset=\"UTF-8\" />\n";
        html << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n";
        html << "  <title>" << title_ << "</title>\n";
        html << "  <link rel=\"stylesheet\" href=\"styles.css\" />\n";
        html << "</head>\n";
        html << "<body>\n";
        html << "  <main class=\"app\">\n";
        html << "    <section class=\"panel\">\n";
        html << "      <h1>" << title_ << "</h1>\n";

        if (!windows_.empty()) {
            html << "      <div class=\"window\">\n";
            for (const auto& window : windows_) {
                html << "        <h2>" << window << "</h2>\n";
            }
            html << "      </div>\n";
        }

        if (!buttons_.empty()) {
            html << "      <div class=\"toolbar\">\n";
            for (const auto& button : buttons_) {
                html << "        <button class=\"action\">" << button << "</button>\n";
            }
            html << "      </div>\n";
        }

        if (!labels_.empty()) {
            html << "      <div id=\"output\" class=\"output\">\n";
            for (const auto& label : labels_) {
                html << "        <p>" << label << "</p>\n";
            }
            html << "      </div>\n";
        }

        html << "    </section>\n";
        html << "  </main>\n";
        html << "  <script src=\"app.js\"></script>\n";
        html << "</body>\n";
        html << "</html>\n";
        return html.str();
    }

    std::string css() const {
        std::ostringstream css;
        css << ":root {\n";
        css << "  --bg: #0f172a;\n";
        css << "  --panel: #111827;\n";
        css << "  --card: #1f2937;\n";
        css << "  --accent: #60a5fa;\n";
        css << "  --text: #e5e7eb;\n";
        css << "  --muted: #94a3b8;\n";
        css << "}\n\n";
        css << "* { box-sizing: border-box; }\n";
        css << "body {\n";
        css << "  margin: 0;\n";
        css << "  font-family: Arial, sans-serif;\n";
        css << "  background: linear-gradient(135deg, #020817, #0f172a 50%, #111827);\n";
        css << "  color: var(--text);\n";
        css << "  min-height: 100vh;\n";
        css << "  display: flex;\n";
        css << "  align-items: center;\n";
        css << "  justify-content: center;\n";
        css << "}\n\n";
        css << ".app { width: min(800px, 90vw); }\n";
        css << ".panel {\n";
        css << "  background: rgba(17, 24, 39, 0.9);\n";
        css << "  border: 1px solid rgba(148, 163, 184, 0.2);\n";
        css << "  border-radius: 18px;\n";
        css << "  padding: 24px;\n";
        css << "  box-shadow: 0 14px 40px rgba(0,0,0,0.35);\n";
        css << "}\n\n";
        css << "h1 { margin-top: 0; font-size: 2rem; }\n";
        css << ".window, .output {\n";
        css << "  background: var(--card);\n";
        css << "  border-radius: 12px;\n";
        css << "  padding: 18px;\n";
        css << "  margin-top: 16px;\n";
        css << "}\n\n";
        css << ".toolbar {\n";
        css << "  display: flex;\n";
        css << "  gap: 12px;\n";
        css << "  flex-wrap: wrap;\n";
        css << "  margin-top: 18px;\n";
        css << "}\n\n";
        css << ".action {\n";
        css << "  border: none;\n";
        css << "  border-radius: 10px;\n";
        css << "  background: var(--accent);\n";
        css << "  color: #031423;\n";
        css << "  padding: 10px 16px;\n";
        css << "  font-weight: 700;\n";
        css << "  cursor: pointer;\n";
        css << "  transition: transform 0.15s ease;\n";
        css << "}\n\n";
        css << ".action:hover { transform: translateY(-1px); }\n";
        css << ".output p { margin: 0; color: var(--muted); font-size: 1rem; }\n";
        return css.str();
    }

    std::string js() const {
        std::ostringstream js;
        js << "document.addEventListener('DOMContentLoaded', () => {\n";
        js << "  const buttons = document.querySelectorAll('.action');\n";
        js << "  const output = document.getElementById('output');\n";
        js << "  buttons.forEach((button) => {\n";
        js << "    button.addEventListener('click', () => {\n";
        js << "      if (output) {\n";
        js << "        const content = button.textContent.trim();\n";
        js << "        output.innerHTML = '<p>Button clicked: ' + content + '</p>';\n";
        js << "      }\n";
        js << "    });\n";
        js << "  });\n";
        js << "});\n";
        return js.str();
    }

    bool exportProject(const std::filesystem::path& outputDir) const {
        try {
            std::filesystem::create_directories(outputDir);
            {
                std::ofstream index(outputDir / "index.html");
                index << html();
            }
            {
                std::ofstream stylesheet(outputDir / "styles.css");
                stylesheet << css();
            }
            {
                std::ofstream script(outputDir / "app.js");
                script << js();
            }
            return true;
        } catch (const std::exception&) {
            return false;
        }
    }

private:
    std::string title_;
    std::vector<std::string> windows_;
    std::vector<std::string> buttons_;
    std::vector<std::string> labels_;
};

}  // namespace cpp2web
