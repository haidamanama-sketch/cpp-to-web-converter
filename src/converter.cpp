#include "cpp2web/converter.hpp"

#include <algorithm>
#include <iostream>

namespace cpp2web {

Converter::Converter(std::string title)
    : title_(std::move(title)) {}

void Converter::fromSourceText(const std::string& source) {
    std::regex windowPattern(R"(\.window\s*\(\s*"([^"]+)"\s*\))");
    std::regex buttonPattern(R"(\.button\s*\(\s*"([^"]+)"\s*\))");
    std::regex labelPattern(R"(\.text\s*\(\s*"([^"]+)"\s*\))");

    auto matches = [](const std::regex& pattern, const std::string& input) {
        std::vector<std::string> values;
        std::sregex_iterator it(input.begin(), input.end(), pattern);
        std::sregex_iterator end;
        for (; it != end; ++it) {
            const std::string value = (*it)[1].str();
            if (!value.empty()) {
                values.push_back(value);
            }
        }
        return values;
    };

    const auto windows = matches(windowPattern, source);
    const auto buttons = matches(buttonPattern, source);
    const auto labels = matches(labelPattern, source);

    for (const auto& window : windows) {
        addWindow(window);
    }
    for (const auto& button : buttons) {
        addButton(button);
    }
    for (const auto& label : labels) {
        addLabel(label);
    }
}

void Converter::addWindow(const std::string& windowName) {
    if (!windowName.empty()) {
        windows_.push_back(windowName);
    }
}

void Converter::addButton(const std::string& buttonLabel) {
    if (!buttonLabel.empty()) {
        buttons_.push_back(buttonLabel);
    }
}

void Converter::addLabel(const std::string& labelText) {
    if (!labelText.empty()) {
        labels_.push_back(labelText);
    }
}

std::string Converter::generateHTML() const {
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

std::string Converter::generateCSS() const {
    return R"css(:root {
  --bg: #0f172a;
  --panel: #111827;
  --card: #1f2937;
  --accent: #60a5fa;
  --text: #e5e7eb;
  --muted: #94a3b8;
}

* { box-sizing: border-box; }

body {
  margin: 0;
  font-family: Arial, sans-serif;
  background: linear-gradient(135deg, #020817, #0f172a 50%, #111827);
  color: var(--text);
  min-height: 100vh;
  display: flex;
  align-items: center;
  justify-content: center;
}

.app { width: min(800px, 90vw); }

.panel {
  background: rgba(17, 24, 39, 0.9);
  border: 1px solid rgba(148, 163, 184, 0.2);
  border-radius: 18px;
  padding: 24px;
  box-shadow: 0 14px 40px rgba(0,0,0,0.35);
}

h1 { margin-top: 0; font-size: 2rem; }

.window, .output {
  background: var(--card);
  border-radius: 12px;
  padding: 18px;
  margin-top: 16px;
}

.toolbar {
  display: flex;
  gap: 12px;
  flex-wrap: wrap;
  margin-top: 18px;
}

.action {
  border: none;
  border-radius: 10px;
  background: var(--accent);
  color: #031423;
  padding: 10px 16px;
  font-weight: 700;
  cursor: pointer;
  transition: transform 0.15s ease;
}

.action:hover { transform: translateY(-1px); }

.output p { margin: 0; color: var(--muted); font-size: 1rem; }
)css";
}

std::string Converter::generateJS() const {
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

bool Converter::writeProject(const std::filesystem::path& outputDir) const {
    try {
        std::filesystem::create_directories(outputDir);

        {
            std::ofstream htmlFile(outputDir / "index.html");
            htmlFile << generateHTML();
        }

        {
            std::ofstream cssFile(outputDir / "styles.css");
            cssFile << generateCSS();
        }

        {
            std::ofstream jsFile(outputDir / "app.js");
            jsFile << generateJS();
        }

        return true;
    } catch (const std::exception&) {
        return false;
    }
}

}  // namespace cpp2web
