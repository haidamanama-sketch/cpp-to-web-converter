#include "cpp2web/converter.hpp"

#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace cpp2web {
namespace {

std::string escapeHTML(const std::string& value) {
    std::string result;
    result.reserve(value.size());
    for (const char character : value) {
        switch (character) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default: result += character; break;
        }
    }
    return result;
}

std::string escapeJavaScriptString(const std::string& value) {
    std::string result;
    for (const char character : value) {
        if (character == '\\' || character == '\'') result += '\\';
        if (character == '\n') result += "\\n";
        else if (character != '\n') result += character;
    }
    return result;
}

template <typename Callback>
void forEachMatch(const std::regex& pattern, const std::string& input, Callback callback) {
    for (std::sregex_iterator it(input.begin(), input.end(), pattern), end; it != end; ++it) {
        callback(*it);
    }
}

}  // namespace

Converter::Converter(std::string title) : title_(std::move(title)) {}

void Converter::fromSourceText(const std::string& source) {
    const std::regex windowPattern(R"(\.window\s*\(\s*"([^"]+)"\s*\))");
    const std::regex buttonPattern(R"(\.button\s*\(\s*"([^"]+)"(?:\s*,\s*"([^"]*)")?\s*\))");
    const std::regex textPattern(R"(\.text\s*\(\s*"([^"]+)"\s*\))");
    const std::regex inputPattern(R"(\.input\s*\(\s*"([^"]+)"(?:\s*,\s*"([^"]*)")?\s*\))");

    forEachMatch(windowPattern, source, [this](const std::smatch& match) { addWindow(match[1].str()); });
    forEachMatch(buttonPattern, source, [this](const std::smatch& match) {
        addButton(match[1].str(), match.size() > 2 ? match[2].str() : std::string{});
    });
    forEachMatch(textPattern, source, [this](const std::smatch& match) { addLabel(match[1].str()); });
    forEachMatch(inputPattern, source, [this](const std::smatch& match) {
        addInput(match[1].str(), match.size() > 2 ? match[2].str() : std::string{});
    });
}

void Converter::addWindow(const std::string& windowName) {
    if (!windowName.empty()) windows_.push_back(windowName);
}

void Converter::addButton(const std::string& buttonLabel, const std::string& action) {
    if (!buttonLabel.empty()) buttons_.push_back({buttonLabel, action});
}

void Converter::addLabel(const std::string& labelText) {
    if (!labelText.empty()) labels_.push_back(labelText);
}

void Converter::addInput(const std::string& id, const std::string& placeholder) {
    if (!id.empty()) inputs_.push_back({id, placeholder});
}

std::string Converter::generateHTML() const {
    std::ostringstream html;
    html << "<!doctype html>\n<html lang=\"en\">\n<head>\n"
         << "  <meta charset=\"UTF-8\">\n"
         << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
         << "  <title>" << escapeHTML(title_) << "</title>\n"
         << "  <link rel=\"stylesheet\" href=\"styles.css\">\n</head>\n<body>\n"
         << "  <main class=\"app\"><section class=\"panel\">\n"
         << "    <h1>" << escapeHTML(title_) << "</h1>\n";

    for (const auto& window : windows_) html << "    <h2>" << escapeHTML(window) << "</h2>\n";
    for (const auto& label : labels_) html << "    <p class=\"description\">" << escapeHTML(label) << "</p>\n";
    if (!inputs_.empty()) {
        html << "    <div class=\"fields\">\n";
        for (const auto& input : inputs_) {
            html << "      <label for=\"" << escapeHTML(input.id) << "\">"
                 << escapeHTML(input.id) << "</label>\n"
                 << "      <input id=\"" << escapeHTML(input.id) << "\" name=\""
                 << escapeHTML(input.id) << "\" placeholder=\"" << escapeHTML(input.placeholder)
                 << "\" autocomplete=\"off\">\n";
        }
        html << "    </div>\n";
    }
    if (!buttons_.empty()) {
        html << "    <div class=\"toolbar\">\n";
        for (std::size_t i = 0; i < buttons_.size(); ++i) {
            html << "      <button type=\"button\" class=\"action\" data-action=\""
                 << escapeHTML(buttons_[i].action) << "\" data-index=\"" << i << "\">"
                 << escapeHTML(buttons_[i].label) << "</button>\n";
        }
        html << "    </div>\n";
    }
    html << "    <output id=\"output\" class=\"output\" aria-live=\"polite\"></output>\n"
         << "  </section></main>\n<script src=\"app.js\"></script>\n</body>\n</html>\n";
    return html.str();
}

std::string Converter::generateCSS() const {
    return R"css(:root { color-scheme: light dark; font-family: system-ui, sans-serif; }
* { box-sizing: border-box; }
body { margin: 0; min-height: 100vh; background: #f3f4f6; color: #172033; }
.app { width: min(720px, 92vw); margin: 8vh auto; }
.panel { background: white; border: 1px solid #d8dee9; border-radius: 12px; padding: 2rem; box-shadow: 0 8px 30px #17203318; }
h1 { margin-top: 0; } h2 { margin-bottom: .25rem; }
.description { color: #526071; }
.fields { display: grid; gap: .45rem; margin: 1.5rem 0; }
input { width: 100%; padding: .7rem .8rem; border: 1px solid #aeb8c7; border-radius: 7px; font: inherit; }
.toolbar { display: flex; flex-wrap: wrap; gap: .7rem; margin-top: 1.25rem; }
.action { padding: .7rem 1rem; border: 0; border-radius: 7px; background: #2563eb; color: white; font: inherit; cursor: pointer; }
.action:hover { background: #1d4ed8; } .action:focus-visible, input:focus-visible { outline: 3px solid #93c5fd; outline-offset: 2px; }
.output { display: block; min-height: 1.5rem; margin-top: 1.25rem; color: #075985; }
@media (prefers-color-scheme: dark) { body { background: #111827; color: #e5e7eb; } .panel { background: #1f2937; border-color: #374151; } .description { color: #b8c2d1; } input { background: #111827; color: inherit; border-color: #4b5563; } }
)css";
}

std::string Converter::generateJS() const {
    std::ostringstream js;
    js << "document.addEventListener('DOMContentLoaded', () => {\n"
       << "  const output = document.querySelector('#output');\n"
       << "  document.querySelectorAll('.action').forEach((button) => {\n"
       << "    button.addEventListener('click', () => {\n"
       << "      const fields = [...document.querySelectorAll('input')];\n"
       << "      const values = fields.map((field) => `${field.name}: ${field.value}`).join(' | ');\n"
       << "      const action = button.dataset.action || `clicked: ${button.textContent.trim()}`;\n"
       << "      output.textContent = values ? `${action} — ${values}` : action;\n"
       << "    });\n"
       << "  });\n"
       << "});\n";
    return js.str();
}

bool Converter::writeProject(const std::filesystem::path& outputDir) const {
    try {
        std::filesystem::create_directories(outputDir);
        std::ofstream htmlFile(outputDir / "index.html");
        std::ofstream cssFile(outputDir / "styles.css");
        std::ofstream jsFile(outputDir / "app.js");
        if (!htmlFile || !cssFile || !jsFile) return false;
        htmlFile << generateHTML(); cssFile << generateCSS(); jsFile << generateJS();
        return static_cast<bool>(htmlFile && cssFile && jsFile);
    } catch (const std::exception&) { return false; }
}

}  // namespace cpp2web
